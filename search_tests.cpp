#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

TEST(CleanToken, punctuationBetween) {
  ASSERT_THAT(cleanToken("hel....lo.."), StrEq("hel....lo"));
  ASSERT_THAT(cleanToken("....hel...lo"), StrEq("hel...lo"));
  ASSERT_THAT(cleanToken("...hel.....lo..."), StrEq("hel.....lo"));
  ASSERT_THAT(cleanToken("hel...lo"), StrEq("hel...lo"));
}

TEST(GatherTokens, leadingSpaces) {
  string text =
      "              red ~gre-en~ orange, yellow + blue (indigo) &violet";
  set<string> expected = {"red",    "gre-en", "orange", "blue",
                          "indigo", "violet", "yellow"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, leadingTrailingSpaces) {
  string text =
      "              red ~gre-en~ orange, yellow + blue (indigo) &violet       "
      "      ";
  set<string> expected = {"red",    "gre-en", "orange", "blue",
                          "indigo", "violet", "yellow"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, trailingSpaces) {
  string text =
      "red ~gre-en~ orange, yellow + blue (indigo) &violet             ";
  set<string> expected = {"red",    "gre-en", "orange", "blue",
                          "indigo", "violet", "yellow"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(BuildIndex, TinyTxt) {
  string filename = "data/tiny.txt";
  map<string, set<string>> expectedIndex = {
      {"eggs", {"www.shoppinglist.com"}},
      {"milk", {"www.shoppinglist.com"}},
      {"fish", {"www.shoppinglist.com", "www.dr.seuss.net"}},
      {"bread", {"www.shoppinglist.com"}},
      {"cheese", {"www.shoppinglist.com"}},
      {"red", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"gre-en", {"www.rainbow.org"}},
      {"orange", {"www.rainbow.org"}},
      {"blue", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"indigo", {"www.rainbow.org"}},
      {"violet", {"www.rainbow.org"}},
      {"yellow", {"www.rainbow.org"}},
      {"one", {"www.rainbow.org"}},
      {"two", {"www.rainbow.org"}},
      {"i'm", {"www.bigbadwolf.com"}},
      {"not", {"www.bigbadwolf.com"}},
      {"trying", {"www.bigbadwolf.com"}},
      {"to", {"www.bigbadwolf.com"}},
      {"eat", {"www.bigbadwolf.com"}},
      {"you", {"www.bigbadwolf.com"}},

  };
  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);

  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(2)) << retTestFeedback;
}
