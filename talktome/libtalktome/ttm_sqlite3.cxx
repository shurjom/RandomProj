/*
 * ttm_sqlite3.cxx
 *
 *  Created on: Jun 15, 2015
 *      Author: Kristofer
 */

#include "ttm_sqlite3.hxx"

#include <iostream>
#include <stdexcept>
#include <sstream>

extern "C" {
#include <sqlite3.h>
#include <unistd.h>
}

class ttm_sqlite3_db {
public:
	virtual ~ttm_sqlite3_db();
	sqlite3 *db;
};

ttm_sqlite3_db::~ttm_sqlite3_db() {
	if (db)
		sqlite3_close(db);
}

ttm_sqlite3::ttm_sqlite3(
		const std::string& filename)
: ttm_sqlite3(filename, false) {
}
ttm_sqlite3::ttm_sqlite3(
		const std::string& filename,
		bool truncate)
: db(new ttm_sqlite3_db()) {
	if (truncate)
	{
		std::cerr << "Removing " << filename << std::endl;
		unlink(filename.c_str());
	}

	std::cerr << "Opening " << filename << std::endl;
	if (sqlite3_open(filename.c_str(), &db->db) != SQLITE_OK)
		throw std::runtime_error(std::string(sqlite3_errmsg(db->db)));
	std::cerr << "Opened" << std::endl;
}
ttm_sqlite3::ttm_sqlite3(
		const std::string& filename,
		bool truncate,
		const std::string& query)
: ttm_sqlite3(filename, truncate) {
	exec(query);
}

void
ttm_sqlite3::exec(const std::string& query)
{
	char *errmsg;
	switch (sqlite3_exec(db->db, query.c_str(), NULL, NULL, &errmsg))
	{
	case SQLITE_OK:
		break;
	default:
		throw std::runtime_error(std::string(errmsg));
	}
}

class ttm_sqlite3_row_impl
: public ttm_sqlite3_row
{
public:
	ttm_sqlite3_row_impl(sqlite3_stmt* stmt)
	: stmt(stmt) {
	}
	int64_t int64(int index) {
		return sqlite3_column_int64(stmt, index);
	}
	std::string text(int index) {
		auto len = sqlite3_column_bytes(stmt, index);
		auto string_start = sqlite3_column_text(stmt, index);
		return std::string(string_start, string_start + len);
	}
private:
	sqlite3_stmt *stmt;
};

class sqlite3_stmt_ptr_wrapper
{
public:
	sqlite3_stmt_ptr_wrapper(sqlite3 *db, const std::string& query, int& ret)
	: stmt(NULL) {
		ret = sqlite3_prepare_v2(db, query.c_str(), query.size() + 1, &stmt, NULL);
	}
	virtual ~sqlite3_stmt_ptr_wrapper() {
		sqlite3_finalize(stmt);
	}
	sqlite3_stmt *stmt;
};

class ttm_sqlite3_stmt_impl
: public ttm_sqlite3_stmt
{
public:
	ttm_sqlite3_stmt_impl(sqlite3 *db, const std::string& query)
	: db(db)
	, stmt(db, query, stmt_prepare_ret) {
		switch (stmt_prepare_ret)
		{
		case SQLITE_OK:
			break;
		default:
			throw std::runtime_error(std::string(sqlite3_errmsg(db)));
		}
		// However, we do not like
	}
	int parameter_index(const std::string& name) {
		auto index = sqlite3_bind_parameter_index(stmt.stmt, name.c_str());
		if (!index)
		{
			std::stringstream s;
			s << "Unable to find parameter by name: " << name << "; there are only " << sqlite3_bind_parameter_count(stmt.stmt) << " parameters.";
			throw std::runtime_error(s.str());
		}
		return index;
	}
	void reset() {
		if (sqlite3_reset(stmt.stmt) != SQLITE_OK)
			throw std::runtime_error(std::string(sqlite3_errmsg(db)));
	}
	void bind(int index, const std::string& value) {
		auto ret = sqlite3_bind_text(stmt.stmt, index, value.c_str(), value.size(), SQLITE_TRANSIENT);
		switch (ret)
		{
		case SQLITE_OK:
			break;
		case SQLITE_RANGE:
		{
			std::stringstream s;
			s << "Index " << index << " is not a valid parameter index. There are only " << sqlite3_bind_parameter_count(stmt.stmt) << " parameters.";
			throw std::runtime_error(s.str());
		}
		default:
		{
			std::stringstream s;
			s << "bind=" << ret;
			throw std::runtime_error(s.str());
		}
		}
	}
	void bind(int index, int64_t value) {
		auto ret = sqlite3_bind_int64(stmt.stmt, index, value);
		switch (ret)
		{
		case SQLITE_OK:
			break;
		case SQLITE_RANGE:
		{
			std::stringstream s;
			s << "Index " << index << " is not a valid parameter index. THere are only " << sqlite3_bind_parameter_count(stmt.stmt) << " parameters.";
			throw std::runtime_error(s.str());
		}
		default:
		{
			std::stringstream s;
			s << "bind=" << ret;
			throw std::runtime_error(s.str());
		}
		}
	}
	void execute() {
		execute([](ttm_sqlite3_row& r){});
	}
	virtual void execute(std::function<void(ttm_sqlite3_row&)> handle_row) {
		bool unfinished_business = true;
		while (unfinished_business) {
			switch (sqlite3_step(stmt.stmt)) {
			case SQLITE_DONE:
				unfinished_business = false;
				break;
			case SQLITE_ROW:
			{
				ttm_sqlite3_row_impl r(stmt.stmt);
				handle_row(r);
			}
			break;
			default:
				throw std::runtime_error(std::string(sqlite3_errmsg(db)));
			}
		}
	}
	virtual ~ttm_sqlite3_stmt_impl() {
	}
private:
	sqlite3 *db;
	int stmt_prepare_ret;
	sqlite3_stmt_ptr_wrapper stmt;
};

ttm_sqlite3_stmt *
ttm_sqlite3::prepare(const std::string& query) {
	return new ttm_sqlite3_stmt_impl(db->db, query);
}

ttm_sqlite3::~ttm_sqlite3() {
}
