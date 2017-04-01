#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <sstream>
#include <string>


extern "C" {
#include <bzlib.h>
#include <fcntl.h>
#include <libxml/SAX2.h>
#include <libxml/xmlstring.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

static void WPcharacters(void *ctx, const xmlChar *ch, int len);
static void WPstartElement(void *ctx, const xmlChar *name, const xmlChar **atts);
static void WPendElement(void *ctx, const xmlChar *name);
}

#include "chunk.hxx"
#include "ttm_sqlite3.hxx"

// Stupid Windows... I thought it was just fopen(), but I was wrong -_-
#ifndef O_BINARY
#define O_BINARY
#endif

using namespace std;

string getResponse(string userEnter);

/* This program will be my first attempt at making "smart" AI that can talk to the user
 * the thing is this AI will be really dumb because this is my first attempt. I mean really dumb....
 */

class article
{
public:
	std::string title;
	std::string text;
};

class article_handler
{
public:
	article_handler()
 	: chunk_fd(open("chunks.dat", O_BINARY|O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR))
	, chunk_writer()
	, db(
			"index.sqlite3",
			true,
			// We're building the database in one go and we always start clean.
			// We're not ab le to resume this operation, so no point in syncing:
			"PRAGMA synchronous = OFF;"
			// Schema
			"CREATE TABLE words (\n"
			"  id INTEGER PRIMARY KEY,\n"
			"  word TEXT NOT NULL UNIQUE,\n"
			"  frequency INTEGER NOT NULL DEFAULT 0);\n"
			"\n"
			"CREATE TABLE word_locations (\n"
			"  word_id INTEGER NOT NULL,\n"
			"  reference_index INTEGER NOT NULL,\n"
			"  reference TEXT NOT NULL);\n"
			)
	, tally_0_stmt(unique_ptr<ttm_sqlite3_stmt>(db.prepare("INSERT OR IGNORE INTO words (word) VALUES (:word);")))
	, tally_0_stmt_word_parameter_index(tally_0_stmt->parameter_index(":word"))
	, tally_1_stmt(unique_ptr<ttm_sqlite3_stmt>(db.prepare("UPDATE words SET frequency = frequency + 1 WHERE word = :word;")))
	, tally_1_stmt_word_parameter_index(tally_1_stmt->parameter_index(":word"))
	, tally_2_stmt(unique_ptr<ttm_sqlite3_stmt>(db.prepare("INSERT INTO word_locations (word_id, reference_index, reference) SELECT w.id, w.frequency - 1, :reference FROM words w WHERE w.word = :word;")))
	, tally_2_stmt_word_parameter_index(tally_2_stmt->parameter_index(":word"))
	, tally_2_stmt_reference_parameter_index(tally_2_stmt->parameter_index(":reference"))
	{
		std::cerr << "handler" << std::endl;
		if (chunk_fd == -1)
			throw std::runtime_error("unable to open chunks.dat for writing.");
	}
	virtual void handle_article(const article& a)
	{
		if (!strncmp(a.text.c_str(), "#REDIR", std::string("#REDIR").size()))
			return;
		std::cout << "Title: " << a.title<< std::endl;

		auto text_str = a.text;
		deWiki(text_str);
		//std::cout << text_str << std::endl << std::endl << std::endl;

		chunk_writer.append(text_str);
		if (chunk_writer.tooBig())
			commit();
	}
	/**
	 * \brief
	 *   Record statistics for an write out the current chunk.
	 */
	void commit() {
		std::cerr << "committing..." << std::endl;
		// Get stats.
		auto chunk_data = chunk_writer.str();
		db.exec("BEGIN TRANSACTION;");
		for (unsigned i = 0; i < chunk_writer.size(); i++)
		{
			auto sentence_reference = chunk_writer.buildReference(i);
			auto words = chunk_writer.at(i).getWords(chunk_data);
			//std::cerr << "Tallying sentence " << sentence_reference << " with " << words.size() << " words." << std::endl;
			for (auto word_iter = words.cbegin(); word_iter != words.cend(); word_iter++)
			{
				// Use prepared statements and stuff.
				tally_0_stmt->reset();
				tally_0_stmt->bind(tally_0_stmt_word_parameter_index, *word_iter);
				tally_0_stmt->execute();

				tally_1_stmt->reset();
				tally_1_stmt->bind(tally_1_stmt_word_parameter_index, *word_iter);
				tally_1_stmt->execute();

				tally_2_stmt->reset();
				tally_2_stmt->bind(tally_2_stmt_word_parameter_index, *word_iter);
				tally_2_stmt->bind(tally_2_stmt_reference_parameter_index, sentence_reference);
				tally_2_stmt->execute();
				//std::cerr << "Tallied " << *word_iter << std::endl;
			}
		}
		db.exec("COMMIT;");
		std::cerr << "chunk had " << chunk_writer.size() << " sentences." << std::endl;

		// Write chunk.
		std::cerr << "going to write to " << chunk_fd << std::endl;
		chunk_writer.write(chunk_fd);
		std::cerr << "commit() finished" << std::endl;
	}
	virtual void write_results(ostream& out)
	{
	}
	void deWiki(string& buf) {
		// Delete things inside of
		deWikiGon(buf, "{{", "}}");
		deWikiGon(buf, "<", ">");
		// Find the actual word inside of []
		deWikiKillua(buf);
		// Delete single quotes that are together
		deWikiPalm(buf, "'''");
		deWikiPalm(buf, "''");
		// Get rid of == signs in subtitles
		deWikiHisoka(buf);
	}
	void deWikiHisoka(string& buf) {
		string::size_type pos, pos_current;
		pos_current = 0;
		while ((pos = buf.find("=", pos_current)) != string::npos) {
			pos_current = pos + 1;
			if (pos == 0 || buf[pos - 1] == '\n') {
				int count_start = 0;
				int count_end = 0;
				while (buf[pos + count_start] == '=') {
					count_start++;
				}
				auto pos_the_beginning_of_the_end = buf.find("\n", pos + count_start);
				if (pos_the_beginning_of_the_end != string::npos)
				{
					if (pos_the_beginning_of_the_end > 0 && buf[pos_the_beginning_of_the_end - 1] == '\r')
						pos_the_beginning_of_the_end--;
					while (pos_the_beginning_of_the_end > 0
							&& buf[pos_the_beginning_of_the_end - 1] == '=') {
						count_end++;
						pos_the_beginning_of_the_end--;
					}
					if (count_start == count_end
							&& pos_the_beginning_of_the_end > pos) {
						buf.erase(pos_the_beginning_of_the_end, count_end);
						buf.erase(pos, count_start);
					}
				}
			}
		}
	}

	void deWikiPalm(string& buf, const string& match) {
		string::size_type pos;
		while ((pos = buf.find(match)) != string::npos) {
			//cout << "Deleted: " << buf.substr(pos, match.size()) << endl;
			buf.erase(pos, match.size());

		}
	}

	void deWikiGon(string& buf, const string& match_start, const string& match_end) {
		string::size_type pos, pos2;
		while ((pos = buf.find(match_start)) != string::npos) {
			pos2 = buf.find(match_end, pos);
			if (pos2 != string::npos) {
				buf.erase(pos, pos2 - pos + match_end.size());
			} else {
				buf.erase(pos, match_start.size());
			}
		}
	}
	void deWikiKillua(string& buf) {
		// []
		deWikiBiscuit(buf, "[[","]]", "|");
		deWikiBiscuit(buf, "[", "]", " ");
	}
	void deWikiBiscuit(string&buf, const string& match_start, const string& match_end, const string& delimiter) {
		string::size_type pos, pos2, pos3;
		while ((pos = buf.find(match_start)) != string::npos) {
			pos2 = buf.find(match_end, pos);

			if (pos2 != string::npos) {
				pos3 = buf.rfind(delimiter, pos2 - 1);
				if (pos3 != string::npos)
					pos3 += delimiter.size();
				if (pos3 == string::npos
						|| pos3 <= pos + match_start.size())
					pos3 = pos + match_start.size();
				//cout << "RESCUED: " << buf.substr(pos3, pos2 - pos3) << endl;
				buf.erase(pos2, match_end.size());
				buf.erase(pos, pos3 - pos);
			} else {
				buf.erase(pos, match_start.size());
			}
		}

	}
	virtual ~article_handler() {
		commit();

		// https://www.sqlite.org/queryplanner.html , word_id cannot be PRIMARY KEY because it isn't UNIQUE.
		// So we have to manually define an index for it.
		// We wait to create the index until the end because http://stackoverflow.com/q/1711631
		// creating the index first and inserting bulk has worse performance than
		// creating at end.
		std::cerr << "Indexing word_locations..." << std::flush;
		db.exec("CREATE INDEX idx__word_locations__word_id ON word_locations(word_id, reference_index);\n");
		std::cerr << " done" << std::endl;

		close(chunk_fd);
	}
private:
	int chunk_fd;
	chunk chunk_writer;
	ttm_sqlite3 db;
	std::unique_ptr<ttm_sqlite3_stmt> tally_0_stmt;
	int tally_0_stmt_word_parameter_index;
	std::unique_ptr<ttm_sqlite3_stmt> tally_1_stmt;
	int tally_1_stmt_word_parameter_index;
	std::unique_ptr<ttm_sqlite3_stmt> tally_2_stmt;
	int tally_2_stmt_word_parameter_index;
	int tally_2_stmt_reference_parameter_index;
};

class article_parser_context
{
private:
	enum state_t {
		ARTICLE_PARSER_STATE_NONE,
		ARTICLE_PARSER_STATE_TITLE,
		ARTICLE_PARSER_STATE_TEXT,
	} state;

	std::stringstream s;
	article current_article;
	article_handler& the_article_handler;

	void push_reading_state(state_t new_state)
	{
		state = new_state;
		s.str(std::string());
	}

	void save_chars(std::string& dest) {
		dest = s.str();
	}
public:
	article_parser_context(article_handler& the_article_handler)
: state(ARTICLE_PARSER_STATE_NONE)
, the_article_handler(the_article_handler)
{
}

	void startElement(const xmlChar *name, const xmlChar **atts) {
		if (xmlStrEqual(name, (xmlChar *)"title"))
			push_reading_state(ARTICLE_PARSER_STATE_TITLE);
		else if (xmlStrEqual(name, (xmlChar *)"text"))
			push_reading_state(ARTICLE_PARSER_STATE_TEXT);
	}
	void characters(const xmlChar *ch, int len) {
		if (state != ARTICLE_PARSER_STATE_NONE)
			s.write((const char *)ch, len);
	}
	void endElement(const xmlChar *name) {
		switch (state) {
		case ARTICLE_PARSER_STATE_TITLE:
			save_chars(current_article.title);
			break;
		case ARTICLE_PARSER_STATE_TEXT:
			save_chars(current_article.text);
			// </text> is the last element, always after the associated <title/>.
			// So, reaching here means we're done with this article.
			// Pass it on to the thing.
			the_article_handler.handle_article(current_article);
			break;
		case ARTICLE_PARSER_STATE_NONE:
			break;
		}
		state = ARTICLE_PARSER_STATE_NONE;
	}
};

static article_parser_context& article_parser_context_cast(void *ctx) {
	return *(article_parser_context *)ctx;
}
static void WPcharacters(void *ctx, const xmlChar *ch, int len) {
	article_parser_context_cast(ctx).characters(ch, len);
}
static void WPstartElement(void *ctx, const xmlChar *name, const xmlChar **atts) {
	article_parser_context_cast(ctx).startElement(name, atts);
}
static void WPendElement(void *ctx, const xmlChar *name){
	article_parser_context_cast(ctx).endElement(name);
}

int main() {
	chunk aChunk;
	string filename = "wiki-pages-articles.bz2";
	char buf[10000];
	auto * bzfile = BZ2_bzopen(filename.c_str(), "rb");
	if (!bzfile)
	{
		std::cerr << "Unable to open “" << filename << "” for reading." << std::endl;
		return 1;
	}
	int bzerr;
	auto len = BZ2_bzRead(&bzerr, bzfile, buf, sizeof(buf)/sizeof(buf[0]) - 1);
	xmlSAXHandler saxHandler;
	memset(&saxHandler, 0, sizeof(saxHandler));
	saxHandler.characters = WPcharacters;
	saxHandler.startElement = WPstartElement;
	saxHandler.endElement = WPendElement;

	std::cerr << "Initializing handler" << std::endl;
	article_handler handler;
	article_parser_context the_article_parser_context(handler);
	auto xmlParserCtxt = xmlCreatePushParserCtxt(&saxHandler, &the_article_parser_context, buf, len, filename.c_str());
	int num_iterations = 10000;

	// Record the time this takes. I should use all of my threads...
	clock_t t1,t2;
	t1 = clock();

	while ((len = BZ2_bzRead(&bzerr, bzfile, buf, sizeof(buf)/sizeof(buf[0]) - 1)) > 0)
	{
		xmlParseChunk(xmlParserCtxt, buf, len, 0);
		if (!--num_iterations)break;
	}
	xmlParseChunk(xmlParserCtxt, buf, 0, 1);
	xmlFreeParserCtxt(xmlParserCtxt);
	BZ2_bzclose(bzfile);
	handler.write_results(cout);
	return 0;
}
