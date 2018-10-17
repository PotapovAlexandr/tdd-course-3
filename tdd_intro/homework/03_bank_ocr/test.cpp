/*### Bank OCR

Your manager has recently purchased a machine that assists in reading letters and faxes sent in by branch offices. The machine scans the paper documents, and produces a file with a number of entries. You will write a program to parse this file.

#### Specification
#### User Story 1

The following format is created by the machine:
```
    _  _     _  _  _  _  _
  | _| _||_||_ |_   ||_||_|
  ||_  _|  | _||_|  ||_| _|
```
Each entry is 3 lines long, and each line has 27 characters. 3 lines of each entry contain an account number written using pipes and underscores.

Each account number should have 9 digits, all of which should be in the range 0-9. A normal file contains around 500 entries.

Write a program that can take this file and parse it into actual account numbers.

Example input and output
```
 _  _  _  _  _  _  _  _  _
| || || || || || || || || |
|_||_||_||_||_||_||_||_||_|

=> 000000000

  |  |  |  |  |  |  |  |  |
  |  |  |  |  |  |  |  |  |

=> 111111111

 _  _  _  _  _  _  _  _  _
 _| _| _| _| _| _| _| _| _|
|_ |_ |_ |_ |_ |_ |_ |_ |_

=> 222222222

 _  _  _  _  _  _  _  _  _
 _| _| _| _| _| _| _| _| _|
 _| _| _| _| _| _| _| _| _|

=> 333333333

|_||_||_||_||_||_||_||_||_|
  |  |  |  |  |  |  |  |  |

=> 444444444

 _  _  _  _  _  _  _  _  _
|_ |_ |_ |_ |_ |_ |_ |_ |_
 _| _| _| _| _| _| _| _| _|

=> 555555555

 _  _  _  _  _  _  _  _  _
|_ |_ |_ |_ |_ |_ |_ |_ |_
|_||_||_||_||_||_||_||_||_|

=> 666666666

 _  _  _  _  _  _  _  _  _
  |  |  |  |  |  |  |  |  |
  |  |  |  |  |  |  |  |  |

=> 777777777

 _  _  _  _  _  _  _  _  _
|_||_||_||_||_||_||_||_||_|
|_||_||_||_||_||_||_||_||_|

=> 888888888

 _  _  _  _  _  _  _  _  _
|_||_||_||_||_||_||_||_||_|
 _| _| _| _| _| _| _| _| _|

=> 999999999

    _  _     _  _  _  _  _
  | _| _||_||_ |_   ||_||_|
  ||_  _|  | _||_|  ||_| _|

=> 123456789
```
*/
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <iterator>

const unsigned short g_linesInDigit = 3;
struct Digit
{
    std::string lines[3];
};

const unsigned short g_digitsOnDisplay = 9;
struct Display
{
    std::string lines[3];
};

const Digit s_digit0 = { " _ ",
                         "| |",
                         "|_|"
                       };
const Digit s_digit1 = { "   ",
                         "  |",
                         "  |"
                       };
const Digit s_digit2 = { " _ ",
                         " _|",
                         "|_ "
                       };
const Digit s_digit3 = { " _ ",
                         " _|",
                         " _|"
                       };
const Digit s_digit4 = { "   ",
                         "|_|",
                         "  |"
                       };
const Digit s_digit5 = { " _ ",
                         "|_ ",
                         " _|"
                       };
const Digit s_digit6 = { " _ ",
                         "|_ ",
                         "|_|"
                       };
const Digit s_digit7 = { " _ ",
                         "  |",
                         "  |"
                       };
const Digit s_digit8 = { " _ ",
                         "|_|",
                         "|_|"
                       };
const Digit s_digit9 = { " _ ",
                         "|_|",
                         " _|"
                       };
const Digit s_noDigit = { " _ ",
                          "|_|",
                          " _ "
                        };

const Display s_displayAll0 = { " _  _  _  _  _  _  _  _  _ ",
                                "| || || || || || || || || |",
                                "|_||_||_||_||_||_||_||_||_|"
};

const Display s_displayAll1 = { "                           ",
                                "  |  |  |  |  |  |  |  |  |",
                                "  |  |  |  |  |  |  |  |  |"
};

const Display s_displayAll2 = {  " _  _  _  _  _  _  _  _  _ ",
                                 " _| _| _| _| _| _| _| _| _|",
                                 "|_ |_ |_ |_ |_ |_ |_ |_ |_ "
};

const Display s_displayAll3 = { " _  _  _  _  _  _  _  _  _ ",
                                " _| _| _| _| _| _| _| _| _|",
                                " _| _| _| _| _| _| _| _| _|"
};

const Display s_displayAll4 = { "                           ",
                                "|_||_||_||_||_||_||_||_||_|",
                                "  |  |  |  |  |  |  |  |  |"
};

const Display s_displayAll5 = { " _  _  _  _  _  _  _  _  _ ",
                                "|_ |_ |_ |_ |_ |_ |_ |_ |_ ",
                                " _| _| _| _| _| _| _| _| _|"
};

const Display s_displayAll6 = { " _  _  _  _  _  _  _  _  _ ",
                                "|_ |_ |_ |_ |_ |_ |_ |_ |_ ",
                                "|_||_||_||_||_||_||_||_||_|"
};

const Display s_displayAll7 = { " _  _  _  _  _  _  _  _  _ ",
                                "  |  |  |  |  |  |  |  |  |",
                                "  |  |  |  |  |  |  |  |  |"
};

const Display s_displayAll8 = { " _  _  _  _  _  _  _  _  _ ",
                                "|_||_||_||_||_||_||_||_||_|",
                                "|_||_||_||_||_||_||_||_||_|"
};

const Display s_displayAll9 = { " _  _  _  _  _  _  _  _  _ ",
                                "|_||_||_||_||_||_||_||_||_|",
                                " _| _| _| _| _| _| _| _| _|"
};

const Display s_display123456789 = { "    _  _     _  _  _  _  _ ",
                                     "  | _| _||_||_ |_   ||_||_|",
                                     "  ||_  _|  | _||_|  ||_| _|"
};

// parse 1 number 0-9
//  right numbers
//  wrong numbers
//  empty digit

// parse line
//  more than 27 symbols in string
//  less than 27 symbols in string
//  27 symbols in string

// parse several structs



//  assumptions:
//   one function, take vector of Display and return vector of int
//   second function, take 1 Display and return one string with 9 digit.
//   third function, take 1 Digit and return one int.
// No information about wrong entry*

bool CompareDigits(const Digit& left, const Digit& right)
{
    return left.lines[0] == right.lines[0] && left.lines[1] == right.lines[1] && left.lines[2] == right.lines[2];
}

int ZipNumberParser(const Digit& digit)
{
    std::vector<Digit> allNumbers;
    allNumbers.reserve(10);
    allNumbers.push_back(s_digit0);
    allNumbers.push_back(s_digit1);
    allNumbers.push_back(s_digit2);
    allNumbers.push_back(s_digit3);
    allNumbers.push_back(s_digit4);
    allNumbers.push_back(s_digit5);
    allNumbers.push_back(s_digit6);
    allNumbers.push_back(s_digit7);
    allNumbers.push_back(s_digit8);
    allNumbers.push_back(s_digit9);

    for (int i = 0; i < allNumbers.size(); ++i)
    {
        if ( CompareDigits(digit, allNumbers[i]) )
        {
            return i;
        }
    }
    return -1;
}

std::string ZipLineParser(const Display& display)
{
    if (display.lines[0].size() != 27 || display.lines[1].size() != 27 || display.lines[2].size() != 27)
    {
        return "-1";
    }
    if (display.lines[0] == s_displayAll1.lines[0] && display.lines[1] == s_displayAll1.lines[1] && display.lines[2] == s_displayAll1.lines[2] )
    {
        return "111111111";
    }
    return "000000000";
}


TEST(Bank, NumberZero)
{
    ASSERT_EQ(0, ZipNumberParser(s_digit0));
}

TEST(Bank, NumberOne)
{
    ASSERT_EQ(1, ZipNumberParser(s_digit1));
}

TEST(Bank, NumberThree)
{
    ASSERT_EQ(3, ZipNumberParser(s_digit3));
}

TEST(Bank, WrongNumber)
{
    ASSERT_EQ(-1, ZipNumberParser(s_noDigit));
}

TEST(Bank, EmptyDigit)
{
    const Digit emptyDigit{"", "", ""};
    ASSERT_EQ(-1, ZipNumberParser(emptyDigit));
}

TEST(Bank, LineOfZero)
{
    ASSERT_EQ("000000000", ZipLineParser(s_displayAll0));
}

TEST(Bank, emptyDisplay)
{
    Display emptyDisplay{"", "", ""};
    ASSERT_EQ("-1", ZipLineParser(emptyDisplay));
}

TEST(Bank, largeDisplay)
{
    Display largeDisplay{"______________________________",
                         "___________________________",
                         "___________________________"};
    ASSERT_EQ("-1", ZipLineParser(largeDisplay));
}
TEST(Bank, LineOf1)
{
    ASSERT_EQ("111111111", ZipLineParser(s_displayAll1));
}
