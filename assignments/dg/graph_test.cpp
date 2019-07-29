/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/

#include "assignments/dg/graph.h"
#include "catch.h"

// TODO(students): Fill this in.

TEST_CASE("Insert Node", "[Temporary Test]") {
  gdwg::Graph<char,int> test;

  test.InsertNode('q');
  REQUIRE(test.IsNode('q') == true);

  test.InsertNode('p');
  REQUIRE(test.IsNode('p') == true);
  REQUIRE(test.InsertEdge('p', 'q', 5) == true);

  std::cout << test;

  test.DeleteNode('q');

  std::cout << test;

  test.InsertNode('x');
  test.InsertNode('y');
  test.InsertEdge('x', 'y', 9);
  test.InsertEdge('y', 'x', -9);
  test.InsertEdge('y', 'x', -99);


  std::cout << test;

  // test.DeleteNode('x');
  //     std::cout << test;


  // test.Replace('x', '&');
  // std::cout << test;

  // test.Clear();
  // std::cout << test;

  REQUIRE(test.IsConnected('x', 'y') == true);

  auto nodes = test.GetNodes();

  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    std::cout << *it << " ";
  }

  std::cout << std::endl;

  test.InsertNode('1');
  test.InsertNode('2');
  test.InsertNode('3');
  test.InsertEdge('y', '1', 1);
  test.InsertEdge('y', '2', 2);
  test.InsertEdge('y', '3', 3);
  std::cout << test;

  auto connected = test.GetConnected('y');
  for (auto it = connected.begin(); it != connected.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;


  auto weights = test.GetWeights('y', 'x');
  for (auto it = weights.begin(); it != weights.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  std::vector<double> v{0.8, 0.11, 0.65, 0.324, 32.234};
  gdwg::Graph<double,double> test2{v.begin(), v.end()};

  std::cout << test2;

  std::string s1{"Hello"};
  std::string s2{"how"};
  std::string s3{"are"};
  auto e1 = std::make_tuple(s1, s2, 5.4);
  auto e2 = std::make_tuple(s2, s3, 7.6);
  auto e = std::vector<std::tuple<std::string, std::string, double>>{e1, e2};
  gdwg::Graph<std::string, double> b{e.begin(), e.end()};

  std::cout << b;

  gdwg::Graph<std::string, double> c{"red", "orange", "yellow", "blue"};
  std::cout << c;

}
