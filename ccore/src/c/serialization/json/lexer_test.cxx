#include <gtest/gtest.h>

#include "charsets/ascii.h"
#include "internal.h"
#include "strings/strings_reader.h"

TEST(JSONLexerTest, TestNextToken) {
	auto symbols = " [\t]\n{\r} :\t,\n";
	auto tts = {
	    JSONTokenArrayBegin,
	    JSONTokenArrayEnd,
	    JSONTokenObjectBegin,
	    JSONTokenObjectEnd,
	    JSONTokenNameSeparator,
	    JSONTokenValueSeparator,
	};
	auto p = json_new_parser(NewStringReader(NewString(symbols, strlen(symbols))), &AsciiCharset);
	for (auto &tt : tts) {
		ASSERT_STREQ(json_token_to_string(tt), json_token_to_string(json_next_token(p)));
	}
}
