#include <gtest/gtest.h>

#include "ccore/strings/strings_reader.h"
#include "internal.h"

TEST(JSONLexerTest, TestNextToken) {
	auto symbols = " [\t]\n{\r} :\t,\n";
	auto tts = {
	    JSONTokenArrayBegin,
	    JSONTokenArrayEnd,
	    JSONTokenObjectBegin,
	    JSONTokenObjectEnd,
	    JSONTokenNameSeparator,
	    JSONTokenValueSeparator,
	    JSONTokenEOF,
	};
	auto p = json_new_parser(NewStringReader(NewString(symbols, strlen(symbols))));
	for (auto &tt : tts) {
		ASSERT_STREQ(json_token_to_string(tt), json_token_to_string(json_next_token(p)));
	}
}
