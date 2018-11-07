/*
We have to develop a coffee machine with TDD. The machine uses the automated source of ingredients, which implements the interface ISourceOfIngredients.
Our machine should be able to produce the coffee accordingly to the next receipts:
- americano: water & coffee 1:2 or 1:3. Water temp 60C
- cappuccino - milk & coffee & milk foam 1:3, 1:3, 1:3. Water temp 80C
- latte - milk & coffee & milk foam 1:4, 1:2, 1:4. Water temp 90C
- marochino - chocolate & coffee & milk foam, 1:4, 1:4, 1:4 and 1:4 is empty
We have 2 possible sizes of the cup:
- little 100 gram
- big 140 gram
Implement worked coffee machine using ISourceOfIngredients to controll the process of coffee production.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class ISourceOfIngredients
{
public:
    virtual ~ISourceOfIngredients() {}
    virtual void SetCupSize(int gram) = 0;
    virtual void AddWater(int gram, int temperature) = 0;
    virtual void AddSugar(int gram) = 0;
    virtual void AddCoffee(int gram) = 0;
    virtual void AddMilk(int gram) = 0;
    virtual void AddMilkFoam(int gram) = 0;
    virtual void AddChocolate(int gram) = 0;
    virtual void AddCream(int gram) = 0;
};


enum Cup
{
    Normal = 100,
    Big = 140
};

enum Coffee
{
    Americano,
    Cappuccino,
    Latte,
    Marochino
};

class MockSourceOfIngredients : public ISourceOfIngredients
{
public:
    MOCK_METHOD1(SetCupSize, void(int));
    MOCK_METHOD2(AddWater, void(int, int));
    MOCK_METHOD1(AddSugar, void(int));
    MOCK_METHOD1(AddCoffee, void(int));
    MOCK_METHOD1(AddMilk, void(int));
    MOCK_METHOD1(AddMilkFoam, void(int));
    MOCK_METHOD1(AddChocolate, void(int));
    MOCK_METHOD1(AddCream, void(int));
};

class CoffeeMachine
{
public:
    CoffeeMachine(ISourceOfIngredients& source) : m_source(source)
    {

    }
    void CreateCoffee(const Cup cup, const Coffee coffee)
    {
        switch (coffee) {
        case Americano:
        {
            MakeAmericano(cup);
            break;
        }
        case Cappuccino:
        {
            m_source.AddCoffee(cup/4);
            m_source.SetCupSize(cup);
            m_source.AddWater(cup/4, 80);
            m_source.AddMilk(cup/4);
            m_source.AddMilkFoam(cup/4);
            break;
        }
        case Latte:
        {
            m_source.AddCoffee(cup/2);
            m_source.SetCupSize(cup);
            m_source.AddWater(cup/4, 90);
            m_source.AddMilk(cup/8);
            m_source.AddMilkFoam(cup/8);
            break;
        }
        case Marochino:
        {
            m_source.AddCoffee(cup/4);
            m_source.SetCupSize(cup);
            m_source.AddMilk(cup/4);
            m_source.AddMilkFoam(cup/4);
            break;
        }
        default:
            break;
        }
    }
private:
    void MakeAmericano(int cupSize)
    {
        m_source.AddCoffee(cupSize/3);
        m_source.SetCupSize(cupSize);
        m_source.AddWater((cupSize/3)*2, 60);
    }
private:
    ISourceOfIngredients& m_source;
};


// Architecture
// Class CoffeMachine
// Class-Mock SourceOfIngredients

// - americano: water & coffee 1:2 or 1:3. Water temp 60C

// Tests list:
// 1. americano + 100 gram = 1 coffe
// 2. americano + 140 gram = 1 large coffee
// 3. AddCoffee, SetCupSize and AddWater calls once
// 4. Check parameters
// 5. Same for each recipe

TEST(CoffeeMachine, CoffemachineIsHere)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);
}

TEST(CoffeeMachine, CallsImportantThings)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(::testing::_)).Times(1);
    EXPECT_CALL(si, SetCupSize(::testing::_)).Times(1);
    EXPECT_CALL(si, AddWater(::testing::_, ::testing::_)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Americano);
}

//- americano: water & coffee 2:3, 1:3. Water temp 60C
//  cup size = 100 ml: 33 coffee + 66 water with 60C
TEST(CoffeeMachine, Americano100ml)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(33)).Times(1);
    EXPECT_CALL(si, SetCupSize(100)).Times(1);
    EXPECT_CALL(si, AddWater(66, 60)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Americano);
}

// cappuccino - milk & coffee & milk foam & water 1:4, 1:4, 1:4 , 1:4. Water temp 80C
// cup size = 100 ml: 25 milk + 25 coffee + 25 milk foam + 25 water 80C
TEST(CoffeeMachine, Cappuccino100ml)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(25)).Times(1);
    EXPECT_CALL(si, SetCupSize(100)).Times(1);
    EXPECT_CALL(si, AddWater(25, 80)).Times(1);
    EXPECT_CALL(si, AddMilk(25)).Times(1);
    EXPECT_CALL(si, AddMilkFoam(25)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Cappuccino);
}

//- latte - milk & coffee & milk foam 1:8, 1:2, 1:8. 1:4 Water temp 90C
// cup size = 100 ml: 12 milk + 50 coffee + 12 milk foam + 25 water 90C
TEST(CoffeeMachine, Latte100ml)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(50)).Times(1);
    EXPECT_CALL(si, SetCupSize(100)).Times(1);
    EXPECT_CALL(si, AddWater(25, 90)).Times(1);
    EXPECT_CALL(si, AddMilk(12)).Times(1);
    EXPECT_CALL(si, AddMilkFoam(12)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Latte);
}

// - marochino - chocolate & coffee & milk foam, 1:4, 1:4, 1:4 and 1:4 is empty
// cup size = 100 ml: 25 chocolate + 25 coffee + 25 milk foam
TEST(CoffeeMachine, Marochino100ml)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(25)).Times(1);
    EXPECT_CALL(si, SetCupSize(100)).Times(1);
    EXPECT_CALL(si, AddMilk(25)).Times(1);
    EXPECT_CALL(si, AddMilkFoam(25)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Marochino);
}

//- americano: water & coffee 1:3 Water temp 60C
//  cup size = 140 ml: 140/3 = 46 coffee + 92 water with 60C
TEST(CoffeeMachine, Americano140ml)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(46)).Times(1);
    EXPECT_CALL(si, SetCupSize(140)).Times(1);
    EXPECT_CALL(si, AddWater(92, 60)).Times(1);

    cm.CreateCoffee(Cup::Big, Coffee::Americano);
}

// cappuccino - milk & coffee & milk foam & water 1:4, 1:4, 1:4 , 1:4. Water temp 80C
// cup size = 140 ml: 35 milk + 35 coffee + 35 milk foam + 35 water 80C
TEST(CoffeeMachine, Cappuccino140ml)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(35)).Times(1);
    EXPECT_CALL(si, SetCupSize(140)).Times(1);
    EXPECT_CALL(si, AddWater(35, 80)).Times(1);
    EXPECT_CALL(si, AddMilk(35)).Times(1);
    EXPECT_CALL(si, AddMilkFoam(35)).Times(1);

    cm.CreateCoffee(Cup::Big, Coffee::Cappuccino);
}

//- latte - milk & coffee & milk foam 1:8, 1:2, 1:8. 1:4 Water temp 90C
// cup size = 140 ml: 17 milk + 70 coffee + 17 milk foam + 35 water 90C
TEST(CoffeeMachine, Latte140ml)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(70)).Times(1);
    EXPECT_CALL(si, SetCupSize(140)).Times(1);
    EXPECT_CALL(si, AddWater(35, 90)).Times(1);
    EXPECT_CALL(si, AddMilk(17)).Times(1);
    EXPECT_CALL(si, AddMilkFoam(17)).Times(1);

    cm.CreateCoffee(Cup::Big, Coffee::Latte);
}

// - marochino - chocolate & coffee & milk foam, 1:4, 1:4, 1:4 and 1:4 is empty
// cup size = 140 ml: 35 chocolate + 35 coffee + 35 milk foam
TEST(CoffeeMachine, Marochino140ml)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(35)).Times(1);
    EXPECT_CALL(si, SetCupSize(140)).Times(1);
    EXPECT_CALL(si, AddMilk(35)).Times(1);
    EXPECT_CALL(si, AddMilkFoam(35)).Times(1);

    cm.CreateCoffee(Cup::Big, Coffee::Marochino);
}
