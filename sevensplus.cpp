#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
using namespace std;
int deck[52] = {0}, endcounter = 0, table[4][3], pfoldtotal = 0, Afoldtotal = 0, check = 0;
class card
{
private:
    int suit;
    int face;
    int input;

public:
    card();
    void shuffle();
    void Swap();
    void prepare();
    void preparedata();
    void startAI();
    void playerturn();
    void computerturn();
    int minimum();
    void AIplay();
    void round();
    void showhand();
    int foldornot(int);
    int playerfold(int);
    void AIfold(int);
    int playcard(int);
    void fun(int, int);
    void showtable();
    void save();
    int leave();
    int newgame();
    void reset();
    void cheatornot();
};
card::card()
{
    suit = face = input = 0;
}
void SetColor(int color = 7)
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
void card::shuffle()
{
    srand(time(NULL));
    for (int i = 0; i < 52; i++)
        deck[i] = i;
    for (int i = 0; i < 1000; i++)
    {
        int r1 = rand() % 52;
        int r2 = rand() % 52;
        int temp = deck[r1];
        deck[r1] = deck[r2];
        deck[r2] = temp;
    }
}
void card::Swap()
{
    int temp = 0;
    for (int j = 0; j < 26; j++)
    {
        for (int i = 0; i < 26; i++)
        {
            if (deck[i] > deck[i + 1])
            {
                temp = deck[i];
                deck[i] = deck[i + 1];
                deck[i + 1] = temp;
            }
        }
    }
}
void card::prepare()
{
    int x = 0;
    shuffle();
    Swap();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 3; j++)
            table[i][j] = -4;
    for (int i = 0; i < 26; i++)
    {
        if (deck[i] == 6)
            x = 1;
    }
    if (x == 0)
        check = 1;
}
void card::preparedata()
{
    ifstream inputcheck("savecheck.txt", ios::in);
    ifstream inputdata("savedata.txt", ios::in);
    int checkdeck[52] = {0}, checkendcount = 0, checkpfold = 0, checkAfold = 0, checktable[4][3], key = 123;
    bool checker = true;
    for (int i = 0; i < 52; i++)
        inputcheck >> checkdeck[i];
    inputcheck >> checkendcount;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 3; j++)
            inputcheck >> checktable[i][j];
    inputcheck >> checkpfold;
    inputcheck >> checkAfold;
    for (int i = 0; i < 52; i++)
    {
        inputdata >> deck[i];
        deck[i] -= key;
    }
    inputdata >> endcounter;
    endcounter -= key;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 3; j++)
        {
            inputdata >> table[i][j];
            table[i][j] -= key;
        }
    inputdata >> pfoldtotal;
    pfoldtotal -= key;
    inputdata >> Afoldtotal;
    Afoldtotal -= key;
    for (int i = 0; i < 52; i++)
        if (checkdeck[i] != deck[i])
            checker = false;
    if (checkendcount != endcounter)
        checker = false;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 3; j++)
            if (checktable[i][j] != table[i][j])
                checker = false;
    if (checkpfold != pfoldtotal)
        checker = false;
    if (checkAfold != Afoldtotal)
        checker = false;
    if (!checker)
    {
        cout << "作弊!!!!" << endl;
        exit(1);
    }
    showtable();
}
int card::newgame()
{
    int input, check = 0;
    while (check == 0)
    {
        cout << "是否開始新遊戲??" << endl;
        cout << "1.新遊戲  2.讀取存檔" << endl;
        cin >> input;
        if (input == 1 || input == 2)
            check = 1;
    }
    if (input == 1)
        return 1;
    else
        return 0;
}
void card::playerturn()
{
    int x, y = 1, input;
    showhand();
    x = foldornot(1);
    while (y == 1)
    {
        cout << "輸入你要出牌的編號 :";
        cin >> input;
        if (x == 0)
            y = playerfold(input);
        else
            y = playcard(input);
    }
    endcounter++;
}
int card::minimum()
{
    int mim = 51, x;
    for (int i = 26; i < 52; i++)
        if (deck[i] != -10)
            if (deck[i] < mim)
            {
                mim = deck[i];
                x = i;
            }
    deck[x] = -10;
    return mim;
}
void card::computerturn()
{
    cout << "電腦回合!!" << endl;
    int x;
    x = foldornot(0);
    if (x == 0)
    {
        AIfold(minimum());
    }
    else
    {
        cout << "電腦出牌!!" << endl;
        AIplay();
    }
    endcounter++;
}
void card::AIplay()
{
    int suit, face;
    if (endcounter == 0)
    {
        for (int i = 26; i < 52; i++)
        {
            face = deck[i] % 13;
            suit = deck[i] / 13;
            if (suit == 0 && face == 6)
            {
                table[suit][1] = 6;
                table[suit][0] = -3;
                table[suit][2] = -3;
                deck[i] = -10;
                return;
            }
        }
    }
    for (int j = 0; j < 4; j++)
        for (int i = 26; i < 52; i++)
            if (deck[i] != -10)
            {
                face = deck[i] % 13;
                suit = deck[i] / 13;
                if (table[j][0] == -3 && suit == j)
                {
                    if (face == table[j][1] - 1)
                    {
                        table[j][0] = face;
                        deck[i] = -10;
                        return;
                    }
                }
                else if (table[j][2] == -3 && suit == j)
                {
                    if (face == table[j][1] + 1)
                    {
                        table[j][2] = face;
                        deck[i] = -10;
                        return;
                    }
                }
                else
                {
                    if (face == table[j][0] - 1 && suit == j)
                    {
                        table[j][0] = face;
                        deck[i] = -10;
                        return;
                    }
                    else if (face == table[j][2] + 1 && suit == j)
                    {
                        table[j][2] = face;
                        deck[i] = -10;
                        return;
                    }
                    else if (face % 13 == 6)
                    {
                        table[suit][1] = face;
                        table[suit][0] = -3;
                        table[suit][2] = -3;
                        deck[i] = -10;
                        return;
                    }
                }
            }
}
void card::round()
{
    if (check == 1)
    {
        computerturn();
        showtable();
        playerturn();
    }
    else if (check == 0)
    {
        playerturn();
        computerturn();
        showtable();
    }
}
int card::leave()
{
    int input, check = 0;
    while (check == 0)
    {
        cout << "是否要結束遊戲??" << endl;
        cout << "離開遊戲  輸入1----繼續遊戲  輸入0" << endl;
        cin >> input;
        if (input == 1 || input == 0)
            check = 1;
    }
    if (input == 1)
    {
        save();
        return 1;
    }
    else
        return 0;
}
void card::save()
{
    ofstream outsavedata("savedata.txt", ios::out);
    ofstream outsavecheck("savecheck.txt", ios::out);
    int key = 123;
    for (int i = 0; i < 52; i++)
        outsavecheck << deck[i] << "  ";
    outsavecheck << endl;
    outsavecheck << endcounter << endl;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 3; j++)
            outsavecheck << table[i][j] << " ";
    outsavecheck << endl
                 << pfoldtotal << endl
                 << Afoldtotal << endl;
    for (int i = 0; i < 52; i++)
        outsavedata << deck[i] + key << "  ";
    outsavedata << endl;
    outsavedata << endcounter + key << endl;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 3; j++)
            outsavedata << table[i][j] + key << " ";
    outsavedata << endl
                << pfoldtotal + key << endl
                << Afoldtotal + key << endl;
}
void card::showhand()
{
    cout << "你的手牌 :" << endl;
    for (int i = 0; i < 26; i++)
    {
        if (deck[i] == -10)
            cout << setw(5) << "已出牌" << endl;
        else
        {
            if (deck[i] / 13 == 0)
            {
                cout << setw(2) << i + 1 << "."
                     << "黑桃" << setw(2) << deck[i] % 13 + 1 << "  ";
                cout << endl;
            }
            else if (deck[i] / 13 == 1)
            {
                cout << setw(2) << i + 1 << "."
                     << "紅心" << setw(2) << deck[i] % 13 + 1 << "  ";
                cout << endl;
            }
            else if (deck[i] / 13 == 2)
            {
                cout << setw(2) << i + 1 << "."
                     << "菱形" << setw(2) << deck[i] % 13 + 1 << "  ";
                cout << endl;
            }
            else
            {
                cout << setw(2) << i + 1 << "."
                     << "梅花" << setw(2) << deck[i] % 13 + 1 << "  ";
                cout << endl;
            }
        }
    }
    cout << endl;
}
int card::foldornot(int x)
{
    int face, suit;
    if (x == 0)
    {
        for (int i = 26; i < 52; i++)
        {
            face = deck[i] % 13;
            suit = deck[i] / 13;
            if (face == 6)
                return 1;
            for (int j = 0; j < 4; j++)
            {
                if (table[j][0] == -3 && suit == j)
                {
                    if (face == table[j][1] - 1)
                        return 1;
                }
                else if (table[j][2] == -3 && suit == j)
                {
                    if (face == table[j][1] + 1)
                        return 1;
                }
                else
                {
                    if (face == table[j][0] - 1 && suit == j)
                        return 1;
                    if (face == table[j][2] + 1 && suit == j)
                        return 1;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < 26; i++)
        {
            face = deck[i] % 13;
            suit = deck[i] / 13;
            if (face == 6)
                return 1;
            for (int j = 0; j < 4; j++)
            {
                if (table[j][0] == -3 && suit == j)
                {
                    if (face == table[j][1] - 1)
                        return 1;
                }
                else if (table[j][2] == -3 && suit == j)
                {
                    if (face == table[j][1] + 1)
                        return 1;
                }
                else
                {
                    if (face == table[j][0] - 1 && suit == j)
                        return 1;
                    if (face == table[j][2] + 1 && suit == j)
                        return 1;
                }
            }
        }
        cout << "玩家fold" << endl;
    }
    return 0;
}
int card::playerfold(int x)
{
    int temp = x;
    x = deck[x - 1] % 13;
    x++;
    cout << "玩家fold點數:" << x << endl;
    deck[temp - 1] = -10;
    pfoldtotal += x;
    cout << "累積點數=" << pfoldtotal << endl;
    return 0;
}
void card::AIfold(int x)
{
    x = x % 13;
    x++;
    cout << "電腦fold點數:" << x << endl;
    Afoldtotal += x;
    cout << "累積點數=" << Afoldtotal << endl;
}
int card::playcard(int x)
{
    int temp = x;
    x = deck[x - 1];
    int suit = x / 13, face = x % 13, check = 0;
    if (endcounter == 0 && table[0][1] == -4)
    {
        if (x != 6)
        {
            cout << "輸入錯誤牌型!!" << endl;
            check = 1;
        }
        else if (x == 6)
        {
            table[suit][1] = 6;
            table[suit][0] = -3;
            table[suit][2] = -3;
        }
    }
    else
    {
        if (x == -10)
        {
            cout << "輸入錯誤牌型!!" << endl;
            check = 1;
        }
        else
        {
            if (face == 6)
            {
                table[suit][1] = 6;
                table[suit][0] = -3;
                table[suit][2] = -3;
            }
            else if (face > 6)
            {
                if (table[suit][2] == -3)
                {
                    if (face == 7)
                        table[suit][2] = 7;
                    else
                    {
                        cout << "輸入錯誤牌型!!" << endl;
                        check = 1;
                    }
                }
                else
                {
                    if (face == table[suit][2] + 1)
                        table[suit][2] = face;
                    else
                    {
                        cout << "輸入錯誤牌型!!" << endl;
                        check = 1;
                    }
                }
            }
            else if (face < 6)
            {
                if (table[suit][0] == -3)
                {
                    if (face == 5)
                        table[suit][0] = 5;
                    else
                    {
                        cout << "輸入錯誤牌型!!" << endl;
                        check = 1;
                    }
                }
                else
                {
                    if (face == table[suit][0] - 1)
                        table[suit][0] = face;
                    else
                    {
                        cout << "輸入錯誤牌型!!" << endl;
                        check = 1;
                    }
                }
            }
        }
    }
    if (check == 1)
        return 1;
    else
    {
        deck[temp - 1] = -10;
        return 0;
    }
}
void card::fun(int a, int b)
{
    if (b == 0)
    {
        if (a < 1)
            cout << "           ";
        else if (a == 1)
            cout << "A"
                 << "          ";
        else if (a == 11)
            cout << "J"
                 << "          ";
        else if (a == 12)
            cout << "Q"
                 << "          ";
        else if (a == 13)
            cout << "K"
                 << "          ";
        else
            cout << left << setw(2) << a << "         " << right;
    }
    else if (b == 1)
    {
        if (a == 0)
            cout << "     *     ";
        else if (a == 1)
            cout << " **     ** ";
        else if (a == 2)
            cout << "     *     ";
        else if (a == 3)
            cout << "     *     ";
    }
    else if (b == 2)
    {
        if (a == 0)
            cout << "   *****   ";
        else if (a == 1)
            cout << "****   ****";
        else if (a == 2)
            cout << "    ***    ";
        else if (a == 3)
            cout << "    ***    ";
    }
    else if (b == 3)
    {
        if (a == 0)
            cout << "  *******  ";
        else if (a == 1)
            cout << " ********* ";
        else if (a == 2)
            cout << "   *****   ";
        else if (a == 3)
            cout << " **  *  ** ";
    }
    else if (b == 4)
    {
        if (a == 0)
            cout << " ********* ";
        else if (a == 1)
            cout << "  *******  ";
        else if (a == 2)
            cout << "  *******  ";
        else if (a == 3)
            cout << "***********";
    }
    else if (b == 5)
    {
        if (a == 0)
            cout << "*   ***   *";
        else if (a == 1)
            cout << "   *****   ";
        else if (a == 2)
            cout << "   *****   ";
        else if (a == 3)
            cout << " **  *  ** ";
    }
    else if (b == 6)
    {
        if (a == 0)
            cout << "     *     ";
        else if (a == 1)
            cout << "    ***    ";
        else if (a == 2)
            cout << "    ***    ";
        else if (a == 3)
            cout << "    ***    ";
    }
    else if (b == 7)
    {
        if (a == 0)
            cout << "    ***    ";
        else if (a == 1)
            cout << "     *     ";
        else if (a == 2)
            cout << "     *     ";
        else if (a == 3)
            cout << "   *****   ";
    }
    else
    {
        if (a < 1)
            cout << "           ";
        else if (a == 1)
            cout << setw(11) << "A";
        else if (a == 11)
            cout << setw(11) << "J";
        else if (a == 12)
            cout << setw(11) << "Q";
        else if (a == 13)
            cout << setw(11) << "K";
        else
            cout << setw(11) << a;
    }
}
void card::showtable()
{
    for (int k = 0; k < 4; k++)
    {
        for (int j = 0; j < 9; j++)
        {
            for (int i = 0; i < 3; i++)
            {
                if (k == 0 || k == 3)
                {
                    SetColor(240);
                    if (j == 0 || j == 8)
                        fun(table[k][i] + 1, j);
                    else
                        fun(k, j);
                    SetColor(7);
                    cout << "  ";
                }
                else if (k == 1 || k == 2)
                {
                    SetColor(252);
                    if (j == 0 || j == 8)
                        fun(table[k][i] + 1, j);
                    else
                        fun(k, j);
                    SetColor(7);
                    cout << "  ";
                }
            }
            cout << endl;
        }
        cout << endl;
    }
}
void card::reset()
{
    ofstream outsavedata("savedata.txt", ios::out);
    for (int i = 0; i < 52; i++)
    {
        deck[i] = i;
        outsavedata << deck[i] << "  ";
    }
    outsavedata << endl;
    endcounter = 0;
    outsavedata << endcounter << endl;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 3; j++)
        {
            table[i][j] = -4;
            outsavedata << table[i][j] << " ";
        }
    pfoldtotal = Afoldtotal = 0;
    outsavedata << endl
                << pfoldtotal << endl
                << Afoldtotal << endl;
}
int main()
{
    COORD Vector;
    Vector.X = 3000;
    Vector.Y = 1000;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), Vector);
    card Card;
    int temp, newgameornot;
    newgameornot = Card.newgame();
    if (newgameornot == 1)
        Card.prepare();
    else
        Card.preparedata();
    while (endcounter < 52)
    {
        Card.round();
        temp = Card.leave();
        if (temp == 1)
            break;
    }
    cout << "EndGame!!" << endl;
    if (endcounter == 52)
    {
        cout << "computer fold=" << Afoldtotal << endl;
        cout << "player fold=" << pfoldtotal << endl;
        if (pfoldtotal > Afoldtotal)
            cout << "YOU LOSE~~" << endl;
        else if (pfoldtotal < Afoldtotal)
            cout << "YOU WIN!!" << endl;
        else
            cout << "DRAW......" << endl;
        Card.reset();
    }
    return 0;
}
