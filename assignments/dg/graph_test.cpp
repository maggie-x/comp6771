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
  test.InsertEdge('p', 'q', 5);
  
}
