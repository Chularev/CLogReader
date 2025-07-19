#include "testclogreader.h"
#include "clogreader.h"

TEST(TestCLogReader, Equal) {
    CLogReader reader;
    EXPECT_TRUE(reader.Open("../data/testfile.txt"));
    reader.SetFilter("hxvG");

    char buf[10];
    EXPECT_TRUE(reader.GetNextLine(buf,10));

    std::string tmp = "+HP6b4SjDQ";
    EXPECT_EQ(std::string(buf), tmp);

    EXPECT_TRUE(reader.GetNextLine(buf,10));
    tmp = "PirGpbO+z5";
    EXPECT_EQ(std::string(buf), tmp);

}

class MyCLogReader : public CLogReader
{
public:
    Search* getFilters()
    {
        return filters;
    }
    int getFiltersLength()
    {
        return filtersLength;
    }
};

TEST(TestCLogReader, FiltersChain) {
    MyCLogReader reader;
    reader.SetFilter("hxvG");

    EXPECT_EQ(1, reader.getFiltersLength());

    Search* filters = reader.getFilters();
    EXPECT_TRUE(std::holds_alternative<Equal>(filters[0]));

}
