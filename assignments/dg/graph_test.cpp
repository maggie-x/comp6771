/*

  == Explanation and rational of testing ==

TODO: 
[ ] change E to a smart pointer
[ ] sort the edges in a smarter way? instead of doing it before we return
[ ] HOW TO TEST individual methods when we don't have access to private fields
    to check if the operation was actually successful
[ ] clean up some code


*/

#include "assignments/dg/graph.h"
#include "catch.h"



#include <algorithm>

// TODO(students): Fill this in.

template<typename T>
bool isEqual(std::vector<T> const &v1, std::vector<T> const &v2)
{
    return (v1.size() == v2.size() &&
            std::equal(v1.begin(), v1.end(), v2.begin()));
}



SCENARIO("a default constructed graph") {
  gdwg::Graph<int, double> graph1;
  gdwg::Graph<char, double> graph2;
  gdwg::Graph<std::vector<double>, double> graph3;

  WHEN("a primitive int is inserted as a node") {
    REQUIRE(graph1.InsertNode(5));
    REQUIRE(graph1.IsNode(5));
  }

  WHEN("a primitive char is inserted as a node") {
    REQUIRE(graph2.InsertNode('c'));
    REQUIRE(graph2.IsNode('c'));
  }

  WHEN("a non-primitive type is inserted as a node") {
    std::vector<double> vector_double{2.0,2.0};
    REQUIRE(graph3.InsertNode(vector_double));
    REQUIRE(graph3.IsNode(vector_double));
  }
}

SCENARIO("a initialiser list constructed graph") {
  gdwg::Graph<std::string, double> graph{"red", "orange", "yellow", "green", "blue", "indigo", "violet"};
  std::cout << graph;

  // since the nodes will be sorted
  std::vector<std::string> colours{"blue", "green", "indigo", "orange", "red", "violet", "yellow" };
  REQUIRE(isEqual(graph.GetNodes(), colours));

  WHEN("multiple edges are added between two nodes") {
    graph.InsertEdge("violet", "yellow", 0.8);
    graph.InsertEdge("violet", "yellow", 0.7);
    graph.InsertEdge("violet", "yellow", 0.6);
    graph.InsertEdge("violet", "yellow", 0.5);
    graph.InsertEdge("violet", "yellow", 0.4);
    graph.InsertEdge("violet", "yellow", 0.3);
    graph.InsertEdge("violet", "yellow", 0.2);
    graph.InsertEdge("violet", "yellow", 0.1);

    // TODO: ENFORCE CHECK THAT THE EDGES ARE IN ORDER 
    std::cout << graph;
  }
}

SCENARIO("A graph initialised with values from a vector (provided by a start and end iterator)") {
  std::string p1 = "hello"; // std::make_pair(1, 'a');
  std::string p2 = "string";// std::make_pair(2, 'b');
  std::string p3 = "cheesecake";// std::make_pair(3, 'c');
  std::vector<std::string> pair_v{p1, p2, p3};
  gdwg::Graph<std::string, double> graph{pair_v.begin(), pair_v.end()};

  REQUIRE(graph.InsertEdge(p1, p2, 0.99));
  REQUIRE(graph.IsConnected(p1, p2));

  WHEN("a node is replaced") {
    auto p4 = "bingsoo";
    std::cout << graph;
    REQUIRE(graph.Replace(p2, p4));
    REQUIRE(!graph.IsNode(p2));

    std::cout << graph;

    // check that all properties of p4 are the same as p2
    // we only change the content of the underlying shared ptr
    // so all the edges should still be there
    REQUIRE(graph.IsNode(p4));
    REQUIRE(graph.IsConnected(p1, p4));
  }

  WHEN("when node is deleted") {
    REQUIRE(graph.DeleteNode(p2));
    REQUIRE(!graph.IsNode(p2));

    // less brittle than just checking returned vector is empty
    auto connected = graph.GetConnected(p1);
    auto it = std::find(connected.begin(), connected.end(), p2);
    REQUIRE(it == connected.end());
    std::cout << graph;
  }

}

SCENARIO("a graph initialised from a vector of tuples in the form <src, dst, weight>") {
  std::string sydney{"sydney"};
  std::string melbourne{"melbourne"};
  std::string perth{"perth"};
  std::string adelaide{"adelaide"};
  std::string brisbane{"brisbane"};


  auto syd_melb = std::make_tuple(sydney, melbourne, 5.4);
  auto melb_per = std::make_tuple(melbourne, perth, 20.1);
  auto per_ade = std::make_tuple(perth, adelaide, 25.9);
  auto syd_ade = std::make_tuple(sydney, adelaide, 4.7);
  auto ade_bris = std::make_tuple(adelaide, brisbane, 2.3);


  auto e = std::vector<std::tuple<std::string, std::string, double>>{syd_melb, melb_per, per_ade, syd_ade, ade_bris};
  gdwg::Graph<std::string, double> aus{e.begin(), e.end()};

  WHEN("we get the nodes of the graph (GetNodes)"){
    auto nodes = aus.GetNodes();
    std::vector<std::string> expected{"adelaide", "brisbane", "melbourne", "perth", "sydney"};
    REQUIRE(isEqual(nodes, expected));
    
    std::cout << aus;
  }

  WHEN("we get the connected outgoing nodes from each city (GetConnected)") {
    auto sydney_expected = std::vector<std::string>{"adelaide", "melbourne"};
    auto sydney_connected = aus.GetConnected("sydney");
    REQUIRE(isEqual(sydney_connected, sydney_expected));

    auto adelaide_expected = std::vector<std::string>{"brisbane"};
    auto adelaide_connected = aus.GetConnected("adelaide");
    REQUIRE(isEqual(adelaide_connected, adelaide_expected));

    auto brisbane_expected = std::vector<std::string>{};
    auto brisbane_connected = aus.GetConnected("brisbane");
    REQUIRE(isEqual(brisbane_connected, brisbane_expected));

    auto melbourne_expected = std::vector<std::string>{"perth"};
    auto melbourne_connected = aus.GetConnected("melbourne");
    REQUIRE(isEqual(melbourne_connected, melbourne_expected));

    auto perth_expected = std::vector<std::string>{"adelaide"};
    auto perth_connected = aus.GetConnected("perth");
    REQUIRE(isEqual(perth_connected, perth_expected));

  }

  WHEN("we get the weights for each connection (GetWeights)") {
    REQUIRE(isEqual(aus.GetWeights("sydney", "melbourne"), std::vector<double>{5.4}));
    REQUIRE(isEqual(aus.GetWeights("melbourne", "perth"), std::vector<double>{20.1}));
    REQUIRE(isEqual(aus.GetWeights("perth", "adelaide"), std::vector<double>{25.9}));
    REQUIRE(isEqual(aus.GetWeights("sydney", "adelaide"), std::vector<double>{4.7}));
    REQUIRE(isEqual(aus.GetWeights("adelaide", "brisbane"), std::vector<double>{2.3}));
  }

  WHEN("we use our custom iterator in the forward direction") {
    std::cout << "-- TESTING CUSTOM ITERATOR --" << std::endl;
    std::cout << aus;
    for (auto it = aus.cbegin(); it != aus.cend(); ++it){
      std::cout << "<" << std::get<0>(*it) << ", " << std::get<1>(*it) << ", " << std::get<2>(*it) << ">" << std::endl;
    }

    std::vector<std::tuple<std::string, std::string, double>> edges(aus.cbegin(), aus.cend());
    auto t1 = std::make_tuple("adelaide", "brisbane", 2.3);
    auto t2 = std::make_tuple("melbourne", "perth", 20.1);
    auto t3 = std::make_tuple("perth", "adelaide", 25.9);
    auto t4 = std::make_tuple("sydney", "adelaide", 4.7);
    auto t5 = std::make_tuple("sydney", "melbourne", 5.4);
    std::vector<std::tuple<std::string, std::string, double>> expected{t1, t2, t3, t4, t5};
    REQUIRE(edges == expected);

  }

  WHEN("we use our custom iterator in the reverse direction") {
    std::cout << "-- TESTING REVERSE CUSTOM ITERATOR --" << std::endl;
    std::cout << aus;
    for (auto it = aus.crbegin(); it != aus.rend(); --it){
      std::cout << "<" << std::get<0>(*it) << ", " << std::get<1>(*it) << ", " << std::get<2>(*it) << ">" << std::endl;
    }

    std::vector<std::tuple<std::string, std::string, double>> edges(aus.cbegin(), aus.cend());
    auto t1 = std::make_tuple("adelaide", "brisbane", 2.3);
    auto t2 = std::make_tuple("melbourne", "perth", 20.1);
    auto t3 = std::make_tuple("perth", "adelaide", 25.9);
    auto t4 = std::make_tuple("sydney", "adelaide", 4.7);
    auto t5 = std::make_tuple("sydney", "melbourne", 5.4);
    std::vector<std::tuple<std::string, std::string, double>> expected{t5, t4, t3, t2, t1};
    REQUIRE(edges == expected);
  }

  // WHEN("we use our custom iterator in the reverse direction") {
  //   for (auto rit = aus.rbegin(); rit != aus.rend(); ++rit) {
  //     std::cout << "<" << std::get<0>(*rit) << ", " << std::get<1>(*rit) << ", " << std::get<2>(*rit) << ">" << std::endl;

  //   }
  // }
}
