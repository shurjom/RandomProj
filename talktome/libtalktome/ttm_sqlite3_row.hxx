/*
 * ttm_sqlite3_row.hxx
 *
 *  Created on: Jun 16, 2015
 *      Author: Kristofer
 */

#ifndef TTM_SQLITE3_ROW_HXX_
#define TTM_SQLITE3_ROW_HXX_

extern "C" {
#include <stdint.h>
}

#include <string>

class ttm_sqlite3_row {
public:
	virtual int64_t int64(int index) = 0;
	virtual std::string text(int index) = 0;
	virtual ~ttm_sqlite3_row();
};

#endif /* TTM_SQLITE3_ROW_HXX_ */
