/*
 * ttm_sqlite3.hxx
 *
 *  Created on: Jun 15, 2015
 *      Author: Kristofer
 */

#ifndef TTM_SQLITE3_HXX_
#define TTM_SQLITE3_HXX_

#include <memory>
#include <string>

#include "ttm_sqlite3_stmt.hxx"

class ttm_sqlite3_db;

class ttm_sqlite3 {
public:
	ttm_sqlite3(const std::string& filename);
	ttm_sqlite3(const std::string& filename, bool truncate);
	ttm_sqlite3(const std::string& filename, bool truncate, const std::string& query);
	void exec(const std::string& query);
	ttm_sqlite3_stmt *prepare(const std::string& query);
	virtual ~ttm_sqlite3();
private:
	std::unique_ptr<ttm_sqlite3_db> db;
};

#endif /* TTM_SQLITE3_HXX_ */
