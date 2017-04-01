/*
 * ttm_sqlite3_stmt.hxx
 *
 *  Created on: Jun 16, 2015
 *      Author: Kristofer
 */

#ifndef TTM_SQLITE3_STMT_HXX_
#define TTM_SQLITE3_STMT_HXX_

#include <functional>
#include <string>

extern "C" {
#include <stdint.h>
}

#include "ttm_sqlite3_row.hxx"

class ttm_sqlite3_stmt {
public:
	virtual int parameter_index(const std::string& name) = 0;
	virtual void reset() = 0;
	virtual void bind(int index, const std::string& value) = 0;
	virtual void bind(int index, int64_t value) = 0;
	virtual void execute() = 0;
	virtual void execute(std::function<void(ttm_sqlite3_row&)> handle_row) = 0;
	virtual ~ttm_sqlite3_stmt();
};

#endif /* TTM_SQLITE3_STMT_HXX_ */
