#include <iostream>
#include <random>
#include <string>

extern "C" {
#include <fcntl.h>
// Silly Windows...
#ifndef O_BINARY
#define O_BINARY 0
#endif
}

#include "chunk.hxx"
#include "sentence.hxx"
#include "ttm_sqlite3.hxx"

class frequent_word
{
public:
	frequent_word(const std::string& word, long long frequency)
	: word(word)
	, frequency(frequency) {
	}
	virtual ~frequent_word() {};

	std::string word;
	long long frequency;
	long long score;
};

int main(int argc, const char *const argv[])
{
	// Initialize source database and open chunk store.
	int chunk_fd = open("chunks.dat", O_RDONLY|O_BINARY);
	chunk chunk_reader;
	ttm_sqlite3 db("index.sqlite3");
	auto frequency_stmt = std::unique_ptr<ttm_sqlite3_stmt>(db.prepare("SELECT w.frequency FROM words w WHERE w.word = :word;"));
	auto frequency_stmt_word_parameter_index = frequency_stmt->parameter_index(":word");

	auto reference_stmt = std::unique_ptr<ttm_sqlite3_stmt>(db.prepare("SELECT wl.reference FROM word_locations wl WHERE wl.word_id = (SELECT w.id FROM words w WHERE w.word = :word) AND wl.reference_index = :reference_index"));
	auto reference_stmt_word_parameter_index = reference_stmt->parameter_index(":word");
	auto reference_stmt_reference_index_parameter_index = reference_stmt->parameter_index(":reference_index");

	std::default_random_engine gen;

	std::string input;
	while (true)
	{
		std::cout << "Gimme an sentance> " << std::flush;
		if (!getline(std::cin, input))
			break;
		// http://stackoverflow.com/q/216823
		auto trim_pos = input.find_last_not_of("\r\n");
		if (trim_pos != std::string::npos)
		input.erase(trim_pos);
		if (!input.size())
			break;

		// Consider as a sentence.
		sentence input_sentence(0, input.size());
		auto words = input_sentence.getWords(input);

		// Rank the words.
		std::vector<frequent_word> frequent_words;
		long long frequency_sum = 0;
		for (auto iter = words.cbegin(); iter != words.cend(); iter++)
		{
			// Lookup frequency.
			int64_t frequency = 0;
			frequency_stmt->reset();
			frequency_stmt->bind(frequency_stmt_word_parameter_index, *iter);
			frequency_stmt->execute([&frequency](ttm_sqlite3_row& r) { frequency = r.int64(0); });
			frequent_words.push_back(frequent_word(*iter, frequency));
			frequency_sum += frequency;
			std::cout << *iter << "=" << frequency << std::endl;
		}
		// Now assign scores of frequency_sum - frequency.
		long long scores_sum = 0;
		for (auto iter = frequent_words.begin(); iter != frequent_words.end(); iter++)
		{
			iter->score = frequency_sum - iter->frequency;
			scores_sum += iter->score;
			std::cout << "s(" << iter->word << ")=" << iter->score << std::endl;
		}
		std::cout << "scores_sum=" << scores_sum << std::endl;

		// Now calculate a random index into scores_sum things.
		// Special case: no words
		if (!frequent_words.size())
		{
			std::cout << "need moar words..." << std::endl;
			continue;
		}
		// Special case: either only one word had any frequency or no words had frequency.
		auto winning_word_iter = frequent_words.cbegin();
		if (!scores_sum)
		{
			for (; winning_word_iter != frequent_words.cend(); winning_word_iter++)
				if (winning_word_iter->frequency)
					break;
			if (winning_word_iter == frequent_words.cend())
			{
				std::cout << "I don't know what you're talkin' 'bout!" << std::endl;
				continue;
			}
		}
		else
		{
			long long winning_score_index = (std::uniform_int_distribution<long long>(0, scores_sum - 1))(gen);
			std::cout << "winning_score_index=" << winning_score_index << std::endl;
			// Now travel into frequent_words until we reach the index.
			for (; winning_word_iter != frequent_words.cend(); winning_word_iter++)
			{
				winning_score_index -= winning_word_iter->score;
				// Consider if word1 had score=1 and word2 had score=1 and winning_word_index=1 (the highest index).
				if (winning_score_index < 0)
					break;
			}
		}
		std::cout << "And the winner is... \"" << winning_word_iter->word << "\"" << std::endl;

		// Calculate the instance of the winning word to use...
		long long winning_word_reference_index = std::uniform_int_distribution<long long>(0, winning_word_iter->frequency - 1)(gen);
		std::cout << "Will use reference index=" << winning_word_reference_index << std::endl;

		reference_stmt->reset();
		reference_stmt->bind(reference_stmt_word_parameter_index, winning_word_iter->word);
		reference_stmt->bind(reference_stmt_reference_index_parameter_index, winning_word_reference_index);
		std::string reference;
		reference_stmt->execute([&reference](ttm_sqlite3_row& r) { reference = r.text(0); });
		std::cout << "reference is " << reference << std::endl;

		chunk_reader.read(chunk_fd, reference);
		std::cerr << "chunk has size " << chunk_reader.size() << std::endl;
		std::cout << chunk_reader.at(reference).get(chunk_reader.str()) << std::endl;
	}
	close(chunk_fd);
	return 0;
}
