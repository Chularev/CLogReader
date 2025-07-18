#include "testclogreader.h"
#include "clogreader.h"

TEST(TestCLogReader, Equal) {
    CLogReader reader;
    EXPECT_TRUE(reader.Open("/home/alex/workspace/CLogReader/data/testfile.txt"));
    reader.SetFilter("HxVG");

    char buf[10];
    EXPECT_TRUE(reader.GetNextLine(buf,10));

    std::string tmp("wJS4HcNIxD");
    EXPECT_EQ(std::string(buf), tmp);

}
