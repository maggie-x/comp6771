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
  
  



}
