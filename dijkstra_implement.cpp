#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <queue>
#include <vector>
using namespace std;

#define MAX 10
const float INF = 1e9;
vector<vector<pair<sf::Vector2f, float>>> graph;
vector<float> dist(MAX, INF);
sf::Vector2f path[MAX];

#define P1 sf::Vector2f(261, 204)
#define P2 sf::Vector2f(490, 347)
#define P3 sf::Vector2f(676, 370)
#define P4 sf::Vector2f(948, 341)
#define P5 sf::Vector2f(1160, 254)
#define P6 sf::Vector2f(1256, 454)
#define P7 sf::Vector2f(718,152)

#define C1 sf::Vector2f(251, 194)
#define C2 sf::Vector2f(480, 337)
#define C3 sf::Vector2f(666, 360)
#define C4 sf::Vector2f(938, 331) 
#define C5 sf::Vector2f(1150, 244)
#define C6 sf::Vector2f(1246, 444)
#define C7 sf::Vector2f(708,142)
#define r 10

#define T1 sf::Vector2f(221, 154)
#define T2 sf::Vector2f(430, 365)
#define T3 sf::Vector2f(645, 322)
#define T4 sf::Vector2f(1025, 325)
#define T5 sf::Vector2f(1162, 212)
#define T6 sf::Vector2f(1256, 454)
#define T7 sf::Vector2f(675,110)
//sf::Vector2f draw_line()
//{
//	return sf::Vector2f(1.5, 2.6);
//}

struct option
{
    bool operator()(const pair < sf::Vector2f , float > & p1, const pair<sf::Vector2f, float>& p2)
    {
        return p1.second > p2.second;
    }
};

class Table
{
    private:
    vector<pair<sf::Vector2f, int>> hst;

    public:
        Table() {
            hst.push_back(make_pair(P1, 0));
            hst.push_back(make_pair(P2, 1));
            hst.push_back(make_pair(P3, 2));
            hst.push_back(make_pair(P4, 3));
            hst.push_back(make_pair(P5, 4));
            hst.push_back(make_pair(P6, 5));
            hst.push_back(make_pair(P7, 6));
        }
        int insert(sf::Vector2f key)
        {
            for (int i = 0; i < hst.size(); i++)
            {
                if (hst[i].first.x == key.x && hst[i].first.y == key.y)
                {
                    return hst[i].second;
                }
            }
        }
        sf::Vector2f insertkey(int a)
        {
            return hst[a].first;
        }
};

/****
***** Function: check_position()
***** Definition: check if pointer of mouse is in position of start and destination points
***** Return value: 'true' if pointer of mouse click is start point either destination point
******/
bool check_position(sf::CircleShape* point, sf::Vector2i mouse_pos, sf::Color color)
{
    if (mouse_pos.x < (*point).getPosition().x + 2*r && mouse_pos.x >(*point).getPosition().x && mouse_pos.y < (*point).getPosition().y + 2*r && mouse_pos.y >(*point).getPosition().y)
    {
        (*point).setFillColor(color);
        return 1;
    }
    else
        return 0;
}

/****
***** Function: check_mouse()
***** Definition: check position of start and destination points
***** Return value: position of start/destination point
******/
sf::Vector2f check_mouse(sf::Vector2i mouse_pos,vector<sf::CircleShape*> circle_arr, sf::Color color)
{
    for (int i = 0; i < circle_arr.size(); i++)
    {
        if (check_position(circle_arr[i], mouse_pos,color) == 1)
        {
            return sf::Vector2f((*circle_arr[i]).getPosition().x + 10, (*circle_arr[i]).getPosition().y + 10);
        }
    }
}

/****
***** Function: change_line_color()
***** Definition: change line to green as indicating the path line
***** Return value: void
******/
void change_line_color(sf::VertexArray* a)
{
    (*a)[0].color = sf::Color::Green;
    (*a)[1].color = sf::Color::Green;
}
/****
***** Function: draw_path()
***** Definition: draw the result path
***** Return value: void
******/
void draw_path(vector<int> result, sf::VertexArray* line_arr[])
{
    Table hst;
    for (int i = result.size()-1; i > 0; i--)
    {
        for (int j = 0; j < 11; j++)
        {
            if (hst.insertkey(result[i]) == (*line_arr[j])[0].position && hst.insertkey(result[i - 1]) == (*line_arr[j])[1].position)
            {
                change_line_color(line_arr[j]);
                break;
            }
            else if (hst.insertkey(result[i-1]) == (*line_arr[j])[0].position && hst.insertkey(result[i]) == (*line_arr[j])[1].position)
            {
                change_line_color(line_arr[j]);
                break;
            }
        }
    }
}

/****
***** Function: change_default_line_color()
***** Definition: reset the line color
***** Return value: void
******/
void change_default_line_color(sf::VertexArray* a)
{
    (*a)[0].color = sf::Color::White;
    (*a)[1].color = sf::Color::White;
}
/****
***** Function: change_defalut_circle_color()
***** Definition: reset the circle color
***** Return value: void
******/
void change_default_circle_color(sf::CircleShape* circle)
{
    (*circle).setFillColor(sf::Color::White);
}

/****
***** Function: reset_program()
***** Definition: reset the program 
***** Return value: void
******/
void reset_program(sf::VertexArray* line_arr[], vector<sf::CircleShape*> circle_arr)
{
    for (int i = 0; i < 11; i++)
    {
        change_default_line_color(line_arr[i]);
    }
    for (int i = 0; i < circle_arr.size(); i++)
    {
        change_default_circle_color(circle_arr[i]);
    }
}


/****
***** Function: Distance()
***** Definition: calculate the value of 2 points
***** Return value: value of the distance
******/
float Distance(sf::Vector2f point1, sf::Vector2f point2)
{
    return sqrt(pow(point1.x - point2.x,2) + pow(point1.y - point2.y,2));
}

/****
***** Function: algorithm()
***** Definition: use Dijkstra algorithm to find the shortest way in the map
***** Return value: void
******/
void algorithm(sf::Vector2f start, sf::Vector2f dest)
{
    Table hst;
    priority_queue<pair<sf::Vector2f, float>, vector<pair<sf::Vector2f, float>>, option> pq;
    pq.push(make_pair(start, 0));
    while (!pq.empty())
    {
        pair<sf::Vector2f, float> top = pq.top();
        pq.pop();
        int u = hst.insert(top.first);
        float w = top.second; // distance 

        for (int i = 0; i < graph[u].size(); ++i)
        {
            pair<sf::Vector2f, float>  neighbor = graph[u][i];
            if (w + neighbor.second < dist[hst.insert(neighbor.first)])
            {
                dist[hst.insert(neighbor.first)] = w + neighbor.second;
                pq.push(pair<sf::Vector2f, float>(neighbor.first, dist[hst.insert(neighbor.first)]));
                cout << "neigbor.first: " << hst.insert(neighbor.first) << endl;
                path[hst.insert(neighbor.first)] = hst.insertkey(u);
            }
        }
    }
}

int main()
{
    //Set up window, event, font
    sf::RenderWindow window(sf::VideoMode(1600, 800), "MAP SIMULATING!");
    sf::Event event;
    sf::Font font;
    sf::Vector2f start;
    sf::Vector2f dest;
    // load the font
    if (!font.loadFromFile("Font/Raleway-Black.ttf"))
    {
        cout << "error" << endl;
    }
    //  set up circle
    sf::CircleShape shape1(r);
    shape1.setPosition(C1);
    sf::CircleShape shape2(r);
    shape2.setPosition(C2);
    sf::CircleShape shape3(r);
    shape3.setPosition(C3);
    sf::CircleShape shape4(r);
    shape4.setPosition(C4);
    sf::CircleShape shape5(r);
    shape5.setPosition(C5);
    sf::CircleShape shape6(r);
    shape6.setPosition(C6);
    sf::CircleShape shape7(r);
    shape7.setPosition(C7);

    // set up lines
    sf::VertexArray line1(sf::LinesStrip, 2);
    line1[0].position = P1;
    line1[1].position = P2;
    sf::VertexArray line2(sf::LinesStrip, 2);
    line2[0].position = P2;
    line2[1].position = P3;
    sf::VertexArray line3(sf::LinesStrip, 2);
    line3[0].position = P3;
    line3[1].position = P4;
    sf::VertexArray line4(sf::LinesStrip, 2);
    line4[0].position = P4;
    line4[1].position = P5;
    sf::VertexArray line5(sf::LinesStrip, 2);
    line5[0].position = P4;
    line5[1].position = P6;
    sf::VertexArray line6(sf::LinesStrip, 2);
    line6[0].position = P1;
    line6[1].position = P7;
    sf::VertexArray line7(sf::LinesStrip, 2);
    line7[0].position = P7;
    line7[1].position = P5;
    sf::VertexArray line8(sf::LinesStrip, 2);
    line8[0].position = P7;
    line8[1].position = P2;
    sf::VertexArray line9(sf::LinesStrip, 2);
    line9[0].position = P7;
    line9[1].position = P4;
    sf::VertexArray line10(sf::LinesStrip, 2);
    line10[0].position = P6;
    line10[1].position = P5;
    sf::VertexArray line11(sf::LinesStrip, 2);
    line11[0].position = P3;
    line11[1].position = P6;
    
    sf::VertexArray* line_arr[11] = { &line1,&line2,&line3,&line4,&line5,&line6,&line7,&line8,&line9,&line10,&line11 };

    vector<sf::CircleShape*> circle_arr;
    circle_arr.push_back(&shape1);
    circle_arr.push_back(&shape2);
    circle_arr.push_back(&shape3);
    circle_arr.push_back(&shape4);
    circle_arr.push_back(&shape5);
    circle_arr.push_back(&shape6);
    circle_arr.push_back(&shape7);

    //set up text
    sf::Text text;
    text.setFillColor(sf::Color::White);
    text.setFont(font);
    text.setString("Tokyo");
    text.setPosition(T1);

    sf::Text text2;
    text2.setFillColor(sf::Color::White);
    text2.setFont(font);
    text2.setString("Hong Kong");
    text2.setPosition(T2);

    sf::Text text3;
    text3.setFillColor(sf::Color::White);
    text3.setFont(font);
    text3.setString("Hanoi");
    text3.setPosition(T3);

    sf::Text text4;
    text4.setFillColor(sf::Color::White);
    text4.setFont(font);
    text4.setString("Ho Chi Minh");
    text4.setPosition(T4);

    sf::Text text5;
    text5.setFillColor(sf::Color::White);
    text5.setFont(font);
    text5.setString("Bangkok");
    text5.setPosition(T5);

    sf::Text text6;
    text6.setFillColor(sf::Color::White);
    text6.setFont(font);
    text6.setString("Singapore");
    text6.setPosition(T6);

    sf::Text text7;
    text7.setFillColor(sf::Color::White);
    text7.setFont(font);
    text7.setString("Seoul");
    text7.setPosition(T7);

#define T8 sf::Vector2f(10,600)
#define T9 sf::Vector2f(10,650)
#define T10 sf::Vector2f(10,700)
    sf::Text text8;
    text8.setFillColor(sf::Color::Cyan);
    text8.setFont(font);
    text8.setString("Left Mouse: Choose start node and destination node ");
    text8.setPosition(T8);

    sf::Text text9;
    text9.setFillColor(sf::Color::Cyan);
    text9.setFont(font);
    text9.setString("Space key: Run the program");
    text9.setPosition(T9);

    sf::Text text10;
    text10.setFillColor(sf::Color::Cyan);
    text10.setFont(font);
    text10.setString("C key: Reset the program");
    text10.setPosition(T10);


    bool start_check = false;
    bool dest_check = false;

    graph = vector<vector<pair<sf::Vector2f, float>>>(MAX + 5, vector<pair<sf::Vector2f, float>>());
    Table hst;
    graph[0].push_back(pair < sf::Vector2f, float>(P2, Distance(hst.insertkey(0), P2)));
    graph[0].push_back(pair < sf::Vector2f, float>(P7, Distance(hst.insertkey(0), P7)));
    graph[1].push_back(pair < sf::Vector2f, float>(P3, Distance(hst.insertkey(1), P3)));
    graph[2].push_back(pair < sf::Vector2f, float>(P4, Distance(hst.insertkey(2), P4)));
    graph[3].push_back(pair < sf::Vector2f, float>(P5, Distance(hst.insertkey(3), P5)));
    graph[3].push_back(pair < sf::Vector2f, float>(P6, Distance(hst.insertkey(3), P6)));
    graph[6].push_back(pair < sf::Vector2f, float>(P5, Distance(hst.insertkey(6), P5))); 
    graph[6].push_back(pair < sf::Vector2f, float>(P1, Distance(hst.insertkey(6), P1)));
    graph[1].push_back(pair < sf::Vector2f, float>(P1, Distance(hst.insertkey(1), P1)));
    graph[2].push_back(pair < sf::Vector2f, float>(P2, Distance(hst.insertkey(2), P2))); 
    graph[3].push_back(pair < sf::Vector2f, float>(P3, Distance(hst.insertkey(3), P3)));
    graph[4].push_back(pair < sf::Vector2f, float>(P4, Distance(hst.insertkey(4), P4)));
    graph[4].push_back(pair < sf::Vector2f, float>(P7, Distance(hst.insertkey(4), P7)));
    graph[5].push_back(pair < sf::Vector2f, float>(P4, Distance(hst.insertkey(5), P4)));
    graph[6].push_back(pair < sf::Vector2f, float>(P2, Distance(hst.insertkey(6), P2)));
    graph[1].push_back(pair < sf::Vector2f, float>(P7, Distance(hst.insertkey(1), P7)));
    graph[6].push_back(pair < sf::Vector2f, float>(P4, Distance(hst.insertkey(6), P4)));
    graph[3].push_back(pair < sf::Vector2f, float>(P7, Distance(hst.insertkey(3), P7)));
    graph[5].push_back(pair < sf::Vector2f, float>(P5, Distance(hst.insertkey(5), P5)));
    graph[4].push_back(pair < sf::Vector2f, float>(P6, Distance(hst.insertkey(4), P6)));
    graph[2].push_back(pair < sf::Vector2f, float>(P6, Distance(hst.insertkey(2), P6)));
    graph[5].push_back(pair < sf::Vector2f, float>(P3, Distance(hst.insertkey(5), P3)));

    // the main execution of program
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            sf::Vector2i localPosition = sf::Mouse::getPosition(window);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (start_check == false)
                {
                    start = check_mouse(localPosition, circle_arr, sf::Color::Red);
                    start_check = true;
                }
                else if (start_check == true && dest_check == false)
                {
                    dest = check_mouse(localPosition, circle_arr, sf::Color::Blue);
                    dest_check = true;
                }
                cout << localPosition.x << " " << localPosition.y <<endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            {
                reset_program(line_arr, circle_arr);
                start_check = false;
                dest_check = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && start_check == true && dest_check == true)
            {
                algorithm(start, dest);
                vector<int> result;
                int i = hst.insert(dest);
                result.push_back(i);
                while (i != hst.insert(start))
                {
                    result.push_back(hst.insert(path[i]));
                    i = hst.insert(path[i]);
                }
                //result.push_back(hst.insert(start));
                for (int i = 0; i < result.size(); i++)
                {
                    cout << result[i] << " ";
                }
                draw_path(result, line_arr);
                start_check = false;
                dest_check = false;
            }
            if (event.type == sf::Event::Closed)
                    window.close();

            window.clear();
            window.draw(line1);
            window.draw(line3);
            window.draw(line2);
            window.draw(line4);
            window.draw(line5);
            window.draw(line6);
            window.draw(line7);
            window.draw(line8);
            window.draw(line9);
            window.draw(line10);
            window.draw(line11);
            window.draw(shape1);
            window.draw(shape2);
            window.draw(shape3);
            window.draw(shape4);
            window.draw(shape5);
            window.draw(shape6);
            window.draw(shape7);
            window.draw(text);
            window.draw(text2);
            window.draw(text3);
            window.draw(text4);
            window.draw(text5);
            window.draw(text6);
            window.draw(text7);
            window.draw(text8);
            window.draw(text9);
            window.draw(text10);
            window.display();
        }
    }
}