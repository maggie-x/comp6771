/*
  == Explanation and rational of testing ==
  The scope of our tests are mainly concerned with ensuring the correctness of each function
  (constructor, overload, method) that we implemented. To do so we created brief tests for each
  function (since each part of the program we implemented is relatively simple, if it works for a
  simple case, it should work for more complicated cases too).

  First, we will test all the constructors and ensure that they work. From there, we just test each
method/operator on any graph, since once the graph is constructed, it will behave the same with the
methods/operators no matter how it was initially constructed.

  Then, we will test edge cases such as calling certain methods/operators on an empty graph, using
non-primitive types, and iterating past the end of the graph. Furthermore, we will also test each
exception to check that it is throwing the correct error and string.

  Overall, each feature should be tested with it's own individual scenario to ensure that it's clear
what is being tested and what could have gone wrong. Each method should be tested with both a const
and non const graph to ensure const correctness (when applicable, no point testing a method which
needs to modify stuff on a const graph)

  The iterator itself and it's functionality (increment and decrement) should be tested too.
  Finally, some tests can be combined or skipped to avoid redundancies. An example is combining cend
and end tests into one, since they literally return the same thing (cend calls and returns end)
*/
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "assignments/dg/graph.h"
#include "catch.h"

template <typename T>
bool isEqual(std::vector<T> const& v1, std::vector<T> const& v2) {
  return (v1.size() == v2.size() && std::equal(v1.begin(), v1.end(), v2.begin()));
}

SCENARIO("Constructing an empty graph using the default constructor))") {
  WHEN("We try to construct a graph of ints and doubles using the default constructor") {
    gdwg::Graph<int, double> g1;
    THEN("The graph should be successfully constructed and empty") {
      std::vector<int> v = g1.GetNodes();
      REQUIRE(v.empty());
    }
  }
}

SCENARIO("Constructing a graph using the const iterator constructor))") {
  WHEN("We try to construct a graph of strings and doubles using the const iterator constructor") {
    std::string p1 = "hello";
    std::string p2 = "string";
    std::string p3 = "cheesecake";
    std::vector<std::string> pair_v{p1, p2, p3};
    gdwg::Graph<std::string, double> g{pair_v.begin(), pair_v.end()};
    THEN("The graph should be successfully constructed and contain the correct nodes in the "
         "correct order") {
      std::vector<std::string> v = g.GetNodes();
      REQUIRE(v[0] == "cheesecake");
      REQUIRE(v[1] == "hello");
      REQUIRE(v[2] == "string");
    }
  }
}

SCENARIO("Constructing a graph using the tuple iterator constructor))") {
  WHEN("We try to construct a graph of strings and doubles using the tuple iterator constructor") {
    std::string sydney{"sydney"};
    std::string melbourne{"melbourne"};
    std::string perth{"perth"};

    auto syd_melb = std::make_tuple(sydney, melbourne, 5.4);
    auto melb_per = std::make_tuple(melbourne, perth, 20.1);

    auto e = std::vector<std::tuple<std::string, std::string, double>>{syd_melb, melb_per};
    gdwg::Graph<std::string, double> aus{e.begin(), e.end()};
    THEN("The graph should be successfully constructed and contain the correct nodes and edges") {
      std::vector<std::string> v = aus.GetNodes();
      REQUIRE(v[0] == melbourne);
      REQUIRE(v[1] == perth);
      REQUIRE(v[2] == sydney);

      std::vector<double> syd_mel_weights = aus.GetWeights(sydney, melbourne);
      std::vector<double> perth_mel_weights = aus.GetWeights(melbourne, perth);
      REQUIRE(syd_mel_weights[0] == 5.4);
      REQUIRE(perth_mel_weights[0] == 20.1);
    }
  }
}

SCENARIO("Constructing a graph using the initializer list constructor))") {
  WHEN("We try to construct a graph of strings and doubles using the tuple iterator constructor") {
    gdwg::Graph<std::string, double> graph{"red",  "orange", "yellow", "green",
                                           "blue", "indigo", "violet"};
    THEN("The graph should be successfully constructed and contain the correct nodes and edges") {
      std::vector<std::string> colours{"blue", "green",  "indigo", "orange",
                                       "red",  "violet", "yellow"};
      REQUIRE(isEqual(graph.GetNodes(), colours));
    }
  }
}

SCENARIO("Construct a new graph from an existing graph with the copy constructor") {
  std::string sydney{"sydney"};
  std::string melbourne{"melbourne"};
  std::string perth{"perth"};

  auto syd_melb = std::make_tuple(sydney, melbourne, 5.4);
  auto melb_per = std::make_tuple(melbourne, perth, 20.1);

  auto e = std::vector<std::tuple<std::string, std::string, double>>{syd_melb, melb_per};
  gdwg::Graph<std::string, double> aus{e.begin(), e.end()};

  WHEN("we construct a new graph from this graph") {
    gdwg::Graph<std::string, double> aus2{aus};
    THEN("the two graphs should equal") { REQUIRE(aus == aus2); }

    WHEN("we change the original graph") {
      aus.InsertNode("random node");
      THEN("the two graphs should be independent of each other") { REQUIRE(aus != aus2); }
    }
  }
}

SCENARIO("Construct a new graph from an existing graph with the move constructor") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to construct a second graph using the move constructor and the first graph") {
      gdwg::Graph<int, double> g2{std::move(g)};
      THEN("The resulting graph should contain the original graphs values, and the original graph "
           "should be empty") {
        std::vector<int> nodes1 = g.GetNodes();
        REQUIRE(nodes1.size() == 0);

        std::vector<int> nodes2 = g2.GetNodes();
        REQUIRE(nodes2.size() == 2);
        REQUIRE(nodes2[0] == 1);
        REQUIRE(nodes2[1] == 2);

        std::vector<double> edges2 = g2.GetWeights(1, 2);
        REQUIRE(edges2.size() == 3);
        REQUIRE(edges2[0] == 4.2);
        REQUIRE(edges2[1] == 5.2);
        REQUIRE(edges2[2] == 6.9);
      }
    }
  }
}

SCENARIO("Testing the copy assignment") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to copy the original graph to a new graph using the copy assignment") {
      gdwg::Graph<int, double> copy = g;
      THEN("The original and copy should contain the same values") {
        std::vector<int> nodes1 = g.GetNodes();
        REQUIRE(nodes1.size() == 2);
        REQUIRE(nodes1[0] == 1);
        REQUIRE(nodes1[1] == 2);

        std::vector<double> edges1 = g.GetWeights(1, 2);
        REQUIRE(edges1.size() == 3);
        REQUIRE(edges1[0] == 4.2);
        REQUIRE(edges1[1] == 5.2);
        REQUIRE(edges1[2] == 6.9);

        std::vector<int> nodes2 = copy.GetNodes();
        REQUIRE(nodes2.size() == 2);
        REQUIRE(nodes2[0] == 1);
        REQUIRE(nodes2[1] == 2);

        std::vector<double> edges2 = copy.GetWeights(1, 2);
        REQUIRE(edges2.size() == 3);
        REQUIRE(edges2[0] == 4.2);
        REQUIRE(edges2[1] == 5.2);
        REQUIRE(edges2[2] == 6.9);
      }

      WHEN("we change the original") {
        g.InsertNode(-99);
        THEN("the copy and the original should be two independent graphs, and changes should not "
             "be reflected in both") {
          REQUIRE(g.IsNode(-99));
          REQUIRE(!copy.IsNode(-99));
        }
      }
    }
  }
}

SCENARIO("Testing the move assignment") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to move the original graph to a new graph using the move assignment") {
      gdwg::Graph<int, double> move = std::move(g);
      THEN("The original should be empty and the new graph should contain the same values that the "
           "original used to") {
        std::vector<int> nodes1 = g.GetNodes();
        REQUIRE(nodes1.size() == 0);

        std::vector<int> nodes2 = move.GetNodes();
        REQUIRE(nodes2.size() == 2);
        REQUIRE(nodes2[0] == 1);
        REQUIRE(nodes2[1] == 2);

        std::vector<double> edges2 = move.GetWeights(1, 2);
        REQUIRE(edges2.size() == 3);
        REQUIRE(edges2[0] == 4.2);
        REQUIRE(edges2[1] == 5.2);
        REQUIRE(edges2[2] == 6.9);
      }
    }
  }
}

// Ensuring that duplicates get removed automatically
SCENARIO("Testing that duplicate nodes and edges get removed") {
  GIVEN("An empty graph of ints and doubles") {
    gdwg::Graph<int, double> g;
    WHEN("We add duplicate edges and nodes") {
      g.InsertNode(1);
      g.InsertNode(1);
      g.InsertNode(2);
      g.InsertEdge(1, 2, 6.9);
      g.InsertEdge(1, 2, 6.9);
      THEN("The resulting graph should automatically remove the duplicates") {
        std::vector<int> nodes = g.GetNodes();
        REQUIRE(nodes.size() == 2);
        REQUIRE(nodes[0] == 1);
        REQUIRE(nodes[1] == 2);
        std::vector<double> edges = g.GetWeights(1, 2);
        REQUIRE(edges.size() == 1);
        REQUIRE(edges[0] == 6.9);
      }
    }
  }
}

SCENARIO("Testing the InsertNode method") {
  GIVEN("An empty graph of int nodes and double edges") {
    gdwg::Graph<int, double> g;
    WHEN("We try to insert a node into it using the InsertNode method") {
      g.InsertNode(5);
      THEN("We should get a graph which contains a single node with value 5") {
        std::vector<int> nodes = g.GetNodes();
        REQUIRE(nodes.size() == 1);
        REQUIRE(nodes[0] == 5);
      }
    }
  }
}

// TESTING THE ITERATOR
SCENARIO("Testing that preincrement works in the custom iterator") {
  GIVEN("A graph of ints and doubles") {
    auto e1 = std::make_tuple(1, 2, 6.9);
    auto e2 = std::make_tuple(1, 2, 4.2);
    auto e3 = std::make_tuple(1, 2, 5.2);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to use the preincrement on an iterator which points to the first element") {
      auto it = g.begin();
      ++it;
      THEN("The iterator should now point to the second element") {
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 5.2));
      }
    }
  }
}

SCENARIO("Testing that postincrement works in the custom iterator") {
  GIVEN("A graph of ints and doubles") {
    auto e1 = std::make_tuple(1, 2, 6.9);
    auto e2 = std::make_tuple(1, 2, 4.2);
    auto e3 = std::make_tuple(1, 2, 5.2);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to use the postincrement on an iterator which points to the first element") {
      auto it = g.begin();
      auto copy = it++;
      THEN("The iterator should now point to the second element and the returned copy should point "
           "to the first element") {
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 5.2));
        REQUIRE(*copy == std::tuple<int, int, double>(1, 2, 4.2));
      }
    }
  }
}

SCENARIO("Testing that predecrement works in the custom iterator") {
  GIVEN("A graph of ints and doubles") {
    auto e1 = std::make_tuple(1, 2, 6.9);
    auto e2 = std::make_tuple(1, 2, 4.2);
    auto e3 = std::make_tuple(1, 2, 5.2);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to use the predecrement on an iterator which points to the second element") {
      auto it = g.find(1, 2, 5.2);
      --it;
      THEN("The iterator should now point to the first element") {
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 4.2));
      }
    }
  }
}

SCENARIO("Testing that postdecrement works in the custom iterator") {
  GIVEN("A graph of ints and doubles") {
    auto e1 = std::make_tuple(1, 2, 6.9);
    auto e2 = std::make_tuple(1, 2, 4.2);
    auto e3 = std::make_tuple(1, 2, 5.2);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to use the postdecrement on an iterator which points to the second element") {
      auto it = g.find(1, 2, 5.2);
      auto copy = it--;
      THEN("The iterator should now point to the first element while the returned copy points to "
           "the second") {
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 4.2));
        REQUIRE(*copy == std::tuple<int, int, double>(1, 2, 5.2));
      }
    }
  }
}

SCENARIO("Testing the InsertNode method with an existing node") {
  GIVEN("A graph containing a single (int) node and double edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(5);
    WHEN("We try to insert an existing node into it using the InsertNode method") {
      g.InsertNode(5);
      THEN("We should get a graph which still contains a single node with value 5") {
        std::vector<int> nodes = g.GetNodes();
        REQUIRE(nodes.size() == 1);
        REQUIRE(nodes[0] == 5);
      }
    }
  }
}

SCENARIO("Testing the InsertEdge method") {
  GIVEN("An graph of int nodes and double edges containing just two nodes") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    WHEN(
        "We try to insert an edge between from one node to the other using the InsertEdge method") {
      g.InsertEdge(1, 2, 2.5);
      THEN("We should get a graph which contains those two nodes and an edge from the first to the "
           "second") {
        std::vector<int> nodes = g.GetNodes();
        REQUIRE(nodes.size() == 2);
        REQUIRE(nodes[0] == 1);
        REQUIRE(nodes[1] == 2);

        std::vector<double> edges = g.GetWeights(1, 2);
        REQUIRE(edges[0] == 2.5);
      }
    }
  }
}

SCENARIO("Testing the InsertEdge method with an edge connecting a node to itself") {
  GIVEN("An graph of int nodes and double edges containing just two nodes") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    WHEN("We try to insert an edge which connects one node to itself") {
      g.InsertEdge(1, 1, 2.5);
      THEN("We should get a graph which contains those two nodes and an edge from the first node "
           "to itself") {
        std::vector<int> nodes = g.GetNodes();
        REQUIRE(nodes.size() == 2);
        REQUIRE(nodes[0] == 1);
        REQUIRE(nodes[1] == 2);

        std::vector<double> edges = g.GetWeights(1, 1);
        REQUIRE(edges[0] == 2.5);
      }
    }
  }
}

SCENARIO("Testing the InsertEdge method with an already existing edge") {
  GIVEN("An graph of int nodes and double edges containing just two nodes and an edge connecting "
        "them") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 4.20);
    WHEN("We try to insert an already existing edge") {
      bool success = g.InsertEdge(1, 2, 4.20);
      THEN("The graph should remain unchanged and InsertEdge method should return false") {
        std::vector<int> nodes = g.GetNodes();
        REQUIRE(nodes.size() == 2);
        REQUIRE(nodes[0] == 1);
        REQUIRE(nodes[1] == 2);

        std::vector<double> edges = g.GetWeights(1, 2);
        REQUIRE(edges[0] == 4.20);
        REQUIRE(edges.size() == 1);

        REQUIRE(success == false);
      }
    }
  }
}

SCENARIO("Testing the DeleteNode method on an existing node") {
  GIVEN("An graph of int nodes and double edges containing just two nodes") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    WHEN("We try to delete the first node using the DeleteNode method") {
      g.DeleteNode(1);
      THEN("We should get a graph which contains only the second node") {
        std::vector<int> nodes = g.GetNodes();
        REQUIRE(nodes.size() == 1);
        REQUIRE(nodes[0] == 2);
      }
    }
  }
}

SCENARIO("Testing the DeleteNode method on a non-existent node") {
  GIVEN("An graph of int nodes and double edges containing just two nodes") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    WHEN("We try to delete a non-existent node using the DeleteNode method") {
      g.DeleteNode(3);
      THEN("Nothing should change in the graph") {
        std::vector<int> nodes = g.GetNodes();
        REQUIRE(nodes.size() == 2);
        REQUIRE(nodes[0] == 1);
        REQUIRE(nodes[1] == 2);
      }
    }
  }
}

SCENARIO("Testing the Replace method") {
  GIVEN("An graph of int nodes and double edges containing just two nodes") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    WHEN("We try to replace the first node using the Replace method") {
      g.Replace(1, 3);
      THEN("The value of the first node should now be set to a different one by the replace "
           "method") {
        std::vector<int> nodes = g.GetNodes();
        REQUIRE(nodes.size() == 2);
        REQUIRE(nodes[0] == 2);
        REQUIRE(nodes[1] == 3);
      }
    }
  }
}

SCENARIO("Testing the Replace method with a value that already exists") {
  GIVEN("An graph of int nodes and double edges containing just two nodes") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    WHEN("We try to replace the first node with an already existing value using the Replace "
         "method") {
      REQUIRE(!g.Replace(1, 2));
      THEN("The value of the first node should remain the same") {
        std::vector<int> nodes = g.GetNodes();
        REQUIRE(nodes.size() == 2);
        REQUIRE(nodes[0] == 1);
        REQUIRE(nodes[1] == 2);
      }
    }
  }
}

SCENARIO("Testing the MergeReplace method") {
  GIVEN("A Graph of strings and doubles") {
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

    auto e = std::vector<std::tuple<std::string, std::string, double>>{syd_melb, melb_per, per_ade,
                                                                       syd_ade, ade_bris};
    gdwg::Graph<std::string, double> aus{e.begin(), e.end()};
    WHEN("We try to use the MergeReplace method on it") {
      aus.MergeReplace(sydney, perth);
      THEN("The resulting graph should have one less node, where the old node and its related "
           "edges are replaced by the new node") {
        std::vector<std::string> nodes = aus.GetNodes();
        REQUIRE(nodes.size() == 4);
        REQUIRE(nodes[0] == adelaide);
        REQUIRE(nodes[1] == brisbane);
        REQUIRE(nodes[2] == melbourne);
        REQUIRE(nodes[3] == perth);

        std::vector<std::string> perth_nodes = aus.GetConnected(perth);
        REQUIRE(perth_nodes.size() == 2);
        REQUIRE(perth_nodes[0] == adelaide);
        REQUIRE(perth_nodes[1] == melbourne);

        std::vector<double> perth_adelaide_weights = aus.GetWeights(perth, adelaide);
        REQUIRE(perth_adelaide_weights.size() == 2);
        REQUIRE(perth_adelaide_weights[0] == 4.7);
        REQUIRE(perth_adelaide_weights[1] == 25.9);

        std::vector<double> perth_melbourne_weights = aus.GetWeights(perth, melbourne);
        REQUIRE(perth_melbourne_weights.size() == 1);
        REQUIRE(perth_melbourne_weights[0] == 5.4);
      }
    }
  }
}

SCENARIO("Testing the MergeReplace method when the old node as a self connecting edge") {
  GIVEN("A graph of strings and doubles, where one node has a self connecting edge") {
    std::string sydney{"sydney"};
    std::string melbourne{"melbourne"};
    std::string perth{"perth"};

    auto syd_melb = std::make_tuple(sydney, melbourne, 5.4);
    auto syd_syd = std::make_tuple(sydney, sydney, 4.2);
    auto per_syd = std::make_tuple(perth, sydney, 6.9);
    auto per_mel = std::make_tuple(perth, melbourne, 6.9);
    auto melb_per = std::make_tuple(melbourne, perth, 20.1);

    auto e = std::vector<std::tuple<std::string, std::string, double>>{syd_syd, syd_melb, melb_per,
                                                                       per_syd, per_mel};
    gdwg::Graph<std::string, double> aus{e.begin(), e.end()};
    WHEN("We try to use the MergeReplace method on the self connecting node") {
      aus.MergeReplace(sydney, melbourne);
      THEN("The resulting graph should have the replacing node now hold the same self connecting "
           "edge") {
        std::vector<std::string> nodes = aus.GetNodes();
        REQUIRE(nodes.size() == 2);

        std::vector<double> mel_weights = aus.GetWeights(melbourne, melbourne);
        REQUIRE(mel_weights.size() == 2);
        REQUIRE(mel_weights[0] == 4.2);
        REQUIRE(mel_weights[1] == 5.4);
      }
    }
  }
}

SCENARIO("Testing the MergeReplace method to ensure it removes duplicate edges") {
  GIVEN("A graph of strings and doubles") {
    std::string sydney{"sydney"};
    std::string melbourne{"melbourne"};
    std::string perth{"perth"};

    auto syd_melb = std::make_tuple(sydney, melbourne, 5.4);
    auto syd_syd = std::make_tuple(sydney, perth, 20.1);
    auto per_syd = std::make_tuple(perth, sydney, 6.9);
    auto per_mel = std::make_tuple(perth, melbourne, 6.9);
    auto melb_per = std::make_tuple(melbourne, perth, 20.1);

    auto e = std::vector<std::tuple<std::string, std::string, double>>{syd_syd, syd_melb, melb_per,
                                                                       per_syd, per_mel};
    gdwg::Graph<std::string, double> aus{e.begin(), e.end()};
    WHEN("We try to use the MergeReplace method so that it will create a duplicate edge") {
      aus.MergeReplace(sydney, melbourne);
      THEN("The resulting graph should automatically remove the duplicate") {
        std::vector<std::string> nodes = aus.GetNodes();
        REQUIRE(nodes.size() == 2);

        std::vector<double> mel_perth_weights = aus.GetWeights(melbourne, perth);
        REQUIRE(mel_perth_weights.size() == 1);
        REQUIRE(mel_perth_weights[0] == 20.1);
      }
    }
  }
}

SCENARIO("Testing the clear method") {
  GIVEN("An graph of int nodes and double edges containing two nodes and an edge") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    WHEN("We try to empty the graph using the clear method") {
      g.Clear();
      THEN("The graph should now be empty") {
        std::vector<int> nodes = g.GetNodes();
        REQUIRE(nodes.size() == 0);
      }
    }
  }
}

SCENARIO("Testing the IsNode method (successful case)") {
  GIVEN("An graph of int nodes and double edges containing two nodes and an edge") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    WHEN("We try to check the existence of an existing node using IsNode") {
      bool exist = g.IsNode(1);
      THEN("The return value should be true") { REQUIRE(exist == true); }
    }
  }
}

SCENARIO("Testing the IsNode method (unsuccessful case)") {
  GIVEN("An graph of int nodes and double edges containing two nodes and an edge") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    WHEN("We try to check the existence of an non-existent node using IsNode") {
      bool exist = g.IsNode(69);
      THEN("The return value should be false") { REQUIRE(exist == false); }
    }
  }
}

SCENARIO("Testing the IsConnected method") {
  GIVEN("A graph of int nodes and double edges containing two nodes and an edge") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    WHEN("We try to check the existence of a connecting edge from the first node to the second "
         "using IsConnected") {
      bool exist = g.IsConnected(1, 2);
      THEN("The return value should be true") { REQUIRE(exist == true); }
    }
  }
}

SCENARIO("Testing the GetNodes method to ensure correct nodes output in correct order") {
  GIVEN("A graph of int nodes and double edges containing two nodes and an edge") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(9);
    g.InsertNode(6);
    g.InsertNode(5);
    g.InsertEdge(1, 6, 6.9);
    WHEN("We try to get a vector of nodes using GetNodes") {
      std::vector<int> nodes = g.GetNodes();
      THEN("The resulting vector should contain all the existing nodes in the correct order") {
        REQUIRE(nodes[0] == 1);
        REQUIRE(nodes[1] == 5);
        REQUIRE(nodes[2] == 6);
        REQUIRE(nodes[3] == 9);
      }
    }
  }
}

SCENARIO("Testing the GetConnected method to ensure correct nodes output in correct order") {
  GIVEN("A graph of int nodes and double edges containing some nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertNode(4);
    g.InsertNode(3);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 3, 4.20);
    g.InsertEdge(1, 4, 5.2);
    WHEN("We try to get a vector of nodes connected to a src using GetConnected") {
      std::vector<int> connected = g.GetConnected(1);
      THEN("The resulting vector should contain all nodes connected to the source by an outgoing "
           "edge, sorted by increasing order") {
        REQUIRE(connected[0] == 2);
        REQUIRE(connected[1] == 3);
        REQUIRE(connected[2] == 4);
      }
    }
  }
}

SCENARIO("Testing the GetWeights method to ensure correct weights output in correct order") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to get a vector of edge weights from a src to a dst using GetWeights") {
      std::vector<double> weights = g.GetWeights(1, 2);
      THEN("The resulting vector should contain edge weights from the src to the dst, sorted by "
           "increasing order") {
        REQUIRE(weights[0] == 4.20);
        REQUIRE(weights[1] == 5.2);
        REQUIRE(weights[2] == 6.9);
      }
    }
  }
}

SCENARIO("Testing the find method with a valid edge") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to get an iterator to an edge found in the graph using the find method") {
      gdwg::Graph<int, double>::const_iterator it = g.find(1, 2, 6.9);
      THEN("The resulting iterator should point to a tuple containing the (src, dst, weight)") {
        REQUIRE(*(it) == std::tuple<int, int, double>(1, 2, 6.9));
      }
    }
  }
}

SCENARIO("Testing the find method with an invalid edge") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to get an iterator to an edge found in the graph using the find method") {
      gdwg::Graph<int, double>::const_iterator it = g.find(1, 2, 7);
      THEN("The resulting iterator should point to cend") { REQUIRE(it == g.cend()); }
    }
  }
}

SCENARIO("Testing the (bool) erase method (successful case)") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to erase an existing edge from the graph using the erase method") {
      bool erase = g.erase(1, 2, 6.9);
      THEN("The erase method should return true and the edge should no longer exist") {
        REQUIRE(erase == true);
        std::vector<double> weights = g.GetWeights(1, 2);
        REQUIRE(weights.size() == 2);
        REQUIRE(weights[0] == 4.20);
        REQUIRE(weights[1] == 5.2);
      }
    }
  }
}

SCENARIO("Testing the (bool) erase method (unsuccessful case)") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    WHEN("We try to erase an non-existent edge from the graph using the erase method") {
      bool erase = g.erase(1, 2, 7);
      THEN("The erase method should return false and the graph should remain unchanged") {
        REQUIRE(erase == false);
        REQUIRE(g.IsNode(1) == true);
        REQUIRE(g.IsNode(2) == true);
        std::vector<double> weights = g.GetWeights(1, 2);
        REQUIRE(weights.size() == 1);
      }
    }
  }
}

SCENARIO("Testing the (const_iterator) erase method (successful case)") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 7);
    WHEN(
        "We try to erase an existing edge from the graph using the (const_iterator) erase method") {
      gdwg::Graph<int, double>::const_iterator target = g.find(1, 2, 6.9);
      gdwg::Graph<int, double>::const_iterator it = g.erase(target);
      THEN("The erase method should return an iterator to the element after the removed one and "
           "the graph should no longer have the erased edge") {
        std::cout << "seghunt 69\n";
        REQUIRE(std::get<0>(*it) == 1);
        std::cout << std::get<0>(*it);
        std::cout << "seghunt 2\n";
        REQUIRE(std::get<1>(*it) == 2);
        std::cout << std::get<1>(*it);
        std::cout << "seghunt 3\n";
        REQUIRE(std::get<2>(*it) == 7);
        std::cout << std::get<2>(*it);
        std::cout << "seghunt 4\n";
        std::cout << "seghunt 5\n";
        REQUIRE(g.IsNode(1) == true);
        REQUIRE(g.IsNode(2) == true);
        std::vector<double> weights = g.GetWeights(1, 2);
        REQUIRE(weights.size() == 1);
        REQUIRE(weights[0] == 7);
      }
    }
  }
}

SCENARIO("Testing the (const_iterator) erase method (unsuccessful case)") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 7);
    WHEN("We try to erase a non-existent edge from the graph using the (const_iterator) erase "
         "method") {
      gdwg::Graph<int, double>::const_iterator target = g.find(1, 2, 4.2);
      gdwg::Graph<int, double>::const_iterator it = g.erase(target);
      THEN("The erase method should return an iterator equivalent to .end and the graph should "
           "remain unchanged") {
        REQUIRE(it == g.end());
        REQUIRE(g.IsNode(1) == true);
        REQUIRE(g.IsNode(2) == true);
        std::vector<double> weights = g.GetWeights(1, 2);
        REQUIRE(weights.size() == 2);
        REQUIRE(weights[0] == 6.9);
        REQUIRE(weights[1] == 7);
      }
    }
  }
}

// only need to test one since begin returns cbegin
SCENARIO("Testing the begin and cbegin methods simultaneously (successful case)") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 7);
    WHEN("We try to access the first element in the graph using the begin method") {
      gdwg::Graph<int, double>::const_iterator it = g.begin();
      THEN("The returned iterator should point to the first element in the graph") {
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 6.9));
      }
      THEN("We should be able to increment the returned iterator") {
        ++it;
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 7));
        THEN("We should be able to decrement the returned iterator back to its original position") {
          --it;
          REQUIRE(*it == std::tuple<int, int, double>(1, 2, 6.9));
        }
      }
    }
  }
}

// only need to test one since begin returns cbegin
SCENARIO("Testing the begin and cbegin methods simultaneously on an empty graph") {
  GIVEN("An empty graph") {
    gdwg::Graph<int, double> g;
    WHEN("We try to access the first element in the graph using the begin method") {
      gdwg::Graph<int, double>::const_iterator it = g.begin();
      THEN("Returned iterator should be equivalent to cbegin") { REQUIRE(it == g.cend()); }
    }
  }
}

// only need to test one since end returns cend
SCENARIO("Testing the end and cend methods simultaneously (successful case)") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 7);
    WHEN("We get an iterator to the past-the-end element in the graph using the end method") {
      gdwg::Graph<int, double>::const_iterator it = g.end();
      THEN("We should be able to decrement the returned iterator so that it now points to the last "
           "element") {
        --it;
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 7));
        THEN("We should be able to increment the returned iterator back to its original position "
             "after decrementing it one more time") {
          --it;
          REQUIRE(*it == std::tuple<int, int, double>(1, 2, 6.9));
          ++it;
          REQUIRE(*it == std::tuple<int, int, double>(1, 2, 7));
        }
      }
    }
  }
}

// only need to test one since end returns cend
SCENARIO("Testing the end and cend methods simultaneously on an empty graph") {
  GIVEN("An empty graph") {
    gdwg::Graph<int, double> g;
    WHEN("We try to access the past-the-end element in the graph using the end method") {
      gdwg::Graph<int, double>::const_iterator it = g.end();
      THEN("Returned iterator should be equivalent to cbegin") { REQUIRE(it == g.cbegin()); }
    }
  }
}

// only need to test one since rbegin returns crbegin
SCENARIO("Testing the crbegin and rbegin methods simultaneously (successful case)") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 7);
    WHEN("We try to access the last element in the graph using the rbegin method") {
      gdwg::Graph<int, double>::const_reverse_iterator it = g.rbegin();
      THEN("The returned iterator should point to the last element in the graph") {
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 7));
      }
      THEN("We should be able to move the returned iterator backwards") {
        ++it;
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 6.9));
        THEN("We should be able to move the returned iterator forward to its original position") {
          --it;
          REQUIRE(*it == std::tuple<int, int, double>(1, 2, 7));
        }
      }
    }
  }
}

// only need to test one since crbegin returns rbegin
SCENARIO("Testing the crbegin and rbegin methods simultaneously on an empty graph") {
  GIVEN("An empty graph") {
    gdwg::Graph<int, double> g;
    WHEN("We try to access the last element in the graph using the rbegin method") {
      gdwg::Graph<int, double>::const_reverse_iterator it = g.rbegin();
      THEN("The iterator should be equivalent to crend") { REQUIRE(it == g.crend()); }
    }
  }
}

// only need to test one since crend returns rend
SCENARIO("Testing the crend and rend methods simultaneously (successful case)") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 7);
    WHEN("We try to access the before-the-first element in the graph using the rend method") {
      gdwg::Graph<int, double>::const_reverse_iterator it = g.rend();
      THEN("We should be able to move the returned iterator forward to the first element") {
        --it;
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 6.9));
        THEN("We should be able to move the returned iterator backward to its original position "
             "after moving it forward one more time") {
          --it;
          REQUIRE(*it == std::tuple<int, int, double>(1, 2, 7));
          ++it;
          REQUIRE(*it == std::tuple<int, int, double>(1, 2, 6.9));
        }
      }
    }
  }
}

// only need to test one since crbegin returns rbegin
SCENARIO("Testing the crend and rend methods simultaneously on an empty graph") {
  GIVEN("An empty graph") {
    gdwg::Graph<int, double> g;
    WHEN("We try to access the before-the-first element in the graph using the rend method") {
      gdwg::Graph<int, double>::const_reverse_iterator it = g.rend();
      THEN("The iterator should be equivalent to crbegin") { REQUIRE(it == g.crbegin()); }
    }
  }
}

// TESTING FRIENDS STARTS HERE !!!!!!!!!!!!!!!! >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
SCENARIO("Testing the == friend operator on two equal graphs") {
  GIVEN("Two graphs containing identical nodes and edges") {
    gdwg::Graph<int, double> g1;
    g1.InsertNode(1);
    g1.InsertNode(2);
    g1.InsertNode(4);
    g1.InsertNode(3);
    g1.InsertEdge(1, 2, 6.9);
    g1.InsertEdge(1, 3, 4.20);
    g1.InsertEdge(1, 4, 5.2);

    gdwg::Graph<int, double> g2;
    g2.InsertNode(1);
    g2.InsertNode(2);
    g2.InsertNode(4);
    g2.InsertNode(3);
    g2.InsertEdge(1, 2, 6.9);
    g2.InsertEdge(1, 3, 4.20);
    g2.InsertEdge(1, 4, 5.2);
    WHEN("We try to use the == friend operator to see if they are equal") {
      bool equal = (g1 == g2);
      THEN("The result should equal true") { REQUIRE(equal == true); }
    }
  }
}

SCENARIO("Testing the == friend operator on two unequal graphs") {
  GIVEN("Two graphs containing different nodes and edges") {
    gdwg::Graph<int, double> g1;
    g1.InsertNode(1);
    g1.InsertNode(2);
    g1.InsertNode(4);
    g1.InsertNode(3);
    g1.InsertEdge(1, 2, 6.9);
    g1.InsertEdge(1, 3, 4.20);
    g1.InsertEdge(1, 4, 5.2);

    gdwg::Graph<int, double> g2;
    g2.InsertNode(1);
    g2.InsertNode(2);
    g2.InsertNode(4);
    g2.InsertNode(3);
    g2.InsertEdge(1, 2, 7);
    g2.InsertEdge(1, 3, 4.20);
    g2.InsertEdge(1, 4, 5.2);
    WHEN("We try to use the == friend operator to see if they are equal") {
      bool equal = (g1 == g2);
      THEN("The result should equal false") { REQUIRE(equal == false); }
    }
  }
}

SCENARIO("Testing the != friend operator on two equal graphs") {
  GIVEN("Two graphs containing identical nodes and edges") {
    gdwg::Graph<int, double> g1;
    g1.InsertNode(1);
    g1.InsertNode(2);
    g1.InsertNode(4);
    g1.InsertNode(3);
    g1.InsertEdge(1, 2, 6.9);
    g1.InsertEdge(1, 3, 4.20);
    g1.InsertEdge(1, 4, 5.2);

    gdwg::Graph<int, double> g2;
    g2.InsertNode(1);
    g2.InsertNode(2);
    g2.InsertNode(4);
    g2.InsertNode(3);
    g2.InsertEdge(1, 2, 6.9);
    g2.InsertEdge(1, 3, 4.20);
    g2.InsertEdge(1, 4, 5.2);
    WHEN("We try to use the != friend operator to see if they are equal") {
      bool equal = (g1 != g2);
      THEN("The result should equal false") { REQUIRE(equal == false); }
    }
  }
}

SCENARIO("Testing the != friend operator on two unequal graphs") {
  GIVEN("Two graphs containing identical nodes and edges") {
    gdwg::Graph<int, double> g1;
    g1.InsertNode(1);
    g1.InsertNode(2);
    g1.InsertNode(4);
    g1.InsertNode(69);
    g1.InsertEdge(1, 2, 6.9);
    g1.InsertEdge(1, 4, 5.2);

    gdwg::Graph<int, double> g2;
    g2.InsertNode(1);
    g2.InsertNode(2);
    g2.InsertNode(4);
    g2.InsertNode(3);
    g2.InsertEdge(1, 2, 6.9);
    g2.InsertEdge(1, 3, 4.20);
    g2.InsertEdge(1, 4, 5.2);
    WHEN("We try to use the != friend operator to see if they are equal") {
      bool equal = (g1 != g2);
      THEN("The result should equal true") { REQUIRE(equal == true); }
    }
  }
}

SCENARIO("Checking that the << friend operator works with a simple graph") {
  GIVEN("A graph containing 2 int nodes and an edge connecting them") {
    gdwg::Graph<int, double> g1;
    g1.InsertNode(1);
    g1.InsertNode(2);
    g1.InsertNode(3);
    g1.InsertEdge(1, 2, 6.9);
    g1.InsertEdge(2, 3, 1.1);
    g1.InsertEdge(3, 2, 1.2);
    g1.InsertEdge(3, 2, 1.4);
    WHEN("We try to use the << operator to print out the graph in order") {
      std::stringstream s;
      s << g1;
      THEN("The output stream should print the contents of the graph in the correct order") {
        std::string expected(
            "1 (\n  2 | 6.9\n)\n2 (\n  3 | 1.1\n)\n3 (\n  2 | 1.2\n  2 | 1.4\n)\n");
        REQUIRE(s.str() == expected);
      }
    }
  }
}

// EXCEPTION TESTING STARTS HERE
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

SCENARIO(
    "Testing for exception in InsertEdge when using on src or dst that doesn't exist in graph") {
  GIVEN("An graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to insert an edge between from one node to a non existent one using the "
         "InsertEdge method") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.InsertEdge(1, 3, 2.5),
            "Cannot call Graph::InsertEdge when either src or dst node does not exist");
      }
    }
    WHEN("We try to insert an edge between from a non existent node to another one using the "
         "InsertEdge method") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.InsertEdge(3, 1, 2.5),
            "Cannot call Graph::InsertEdge when either src or dst node does not exist");
      }
    }
    WHEN("We try to insert an edge between two non existent nodes using the InsertEdge method") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.InsertEdge(96, 69, 2.5),
            "Cannot call Graph::InsertEdge when either src or dst node does not exist");
      }
    }
  }
}

SCENARIO("Testing for exception in Replace when the node being replaced doesn't exist in graph") {
  GIVEN("An graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to replace a non existent node using the Replace method") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(g.Replace(3, 1),
                            "Cannot call Graph::Replace on a node that doesn't exist");
      }
    }
  }
}

SCENARIO("Testing for exception in MergeReplace when either node doesn't exist in graph") {
  GIVEN("An graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to replace a non existent node with an existing one using the MergeReplace "
         "method") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.MergeReplace(3, 1),
            "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
      }
    }
    WHEN("We try to replace an existing node with a non existent one using the MergeReplace "
         "method") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.MergeReplace(1, 3),
            "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
      }
    }
    WHEN("We try to replace an non existent node with a non existent one using the MergeReplace "
         "method") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.MergeReplace(69, 3),
            "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
      }
    }
  }
}

SCENARIO("Testing for exception in IsConnected when either node doesn't exist in graph") {
  GIVEN("An graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to use the IsConnected method from a non existent node to an existing one") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.IsConnected(3, 1),
            "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
      }
    }
    WHEN("We try to use the IsConnected method from an existing node to a non existent one") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.IsConnected(1, 3),
            "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
      }
    }
    WHEN("We try to use the IsConnected method between two non existent nodes") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.IsConnected(69, 3),
            "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
      }
    }
  }
}

SCENARIO("Testing for exception in GetConnected when the src node doesn't exist in graph") {
  GIVEN("An graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to use the GetConnected method with a non existent src node") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(g.GetConnected(3),
                            "Cannot call Graph::GetConnected if src doesn't exist in the graph");
      }
    }
  }
}

SCENARIO("Testing for exception in GetWeights when the src or dst node doesn't exist in graph") {
  GIVEN("An graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 5.2);
    g.InsertEdge(1, 2, 4.20);
    WHEN("We try to use the GetWeights method with a non existent src node") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.GetWeights(3, 1),
            "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
      }
    }
    WHEN("We try to use the GetWeights method with a non existent dst node") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.GetWeights(1, 3),
            "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
      }
    }
    WHEN("We try to use the GetWeights method with a non existent src and dst node") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.GetWeights(3, 69),
            "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
      }
    }
  }
}

// DUPLICATING PREVIOUS TESTS WITH CONST GRAPHS FOR CONST CORRECTNESS (where applicable, no point
// testing const on non const functions or functions that need to change the graph such as
// InsertNode)

SCENARIO("Constructing an empty const graph using the default constructor))") {
  WHEN("We try to construct a graph of ints and doubles using the default constructor") {
    const gdwg::Graph<int, double> g1;
    THEN("The graph should be successfully constructed and empty") {
      std::vector<int> v = g1.GetNodes();
      REQUIRE(v.empty());
    }
  }
}

SCENARIO("Constructing a const graph using the const iterator constructor))") {
  WHEN("We try to construct a graph of strings and doubles using the const iterator constructor") {
    std::string p1 = "hello";
    std::string p2 = "string";
    std::string p3 = "cheesecake";
    std::vector<std::string> pair_v{p1, p2, p3};
    const gdwg::Graph<std::string, double> g{pair_v.begin(), pair_v.end()};
    THEN("The graph should be successfully constructed and contain the correct nodes in the "
         "correct order") {
      std::vector<std::string> v = g.GetNodes();
      REQUIRE(v[0] == "cheesecake");
      REQUIRE(v[1] == "hello");
      REQUIRE(v[2] == "string");
    }
  }
}

SCENARIO("Constructing a const graph using the tuple iterator constructor))") {
  WHEN("We try to construct a graph of strings and doubles using the tuple iterator constructor") {
    std::string sydney{"sydney"};
    std::string melbourne{"melbourne"};
    std::string perth{"perth"};

    auto syd_melb = std::make_tuple(sydney, melbourne, 5.4);
    auto melb_per = std::make_tuple(melbourne, perth, 20.1);

    auto e = std::vector<std::tuple<std::string, std::string, double>>{syd_melb, melb_per};
    const gdwg::Graph<std::string, double> aus{e.begin(), e.end()};
    THEN("The graph should be successfully constructed and contain the correct nodes and edges") {
      std::vector<std::string> v = aus.GetNodes();
      REQUIRE(v[0] == melbourne);
      REQUIRE(v[1] == perth);
      REQUIRE(v[2] == sydney);

      std::vector<double> syd_mel_weights = aus.GetWeights(sydney, melbourne);
      std::vector<double> perth_mel_weights = aus.GetWeights(melbourne, perth);
      REQUIRE(syd_mel_weights[0] == 5.4);
      REQUIRE(perth_mel_weights[0] == 20.1);
    }
  }
}

SCENARIO("Constructing a const graph using the initializer list constructor))") {
  WHEN("We try to construct a graph of strings and doubles using the tuple iterator constructor") {
    const gdwg::Graph<std::string, double> graph{"red",  "orange", "yellow", "green",
                                                 "blue", "indigo", "violet"};
    THEN("The graph should be successfully constructed and contain the correct nodes and edges") {
      std::vector<std::string> colours{"blue", "green",  "indigo", "orange",
                                       "red",  "violet", "yellow"};
      REQUIRE(isEqual(graph.GetNodes(), colours));
    }
  }
}

SCENARIO("Construct a new const graph from an existing graph with the copy constructor") {
  std::string sydney{"sydney"};
  std::string melbourne{"melbourne"};
  std::string perth{"perth"};

  auto syd_melb = std::make_tuple(sydney, melbourne, 5.4);
  auto melb_per = std::make_tuple(melbourne, perth, 20.1);

  auto e = std::vector<std::tuple<std::string, std::string, double>>{syd_melb, melb_per};
  const gdwg::Graph<std::string, double> aus{e.begin(), e.end()};

  WHEN("we construct a new graph from this graph") {
    gdwg::Graph<std::string, double> aus2{aus};

    THEN("the two graphs should equal") { REQUIRE(aus == aus2); }
  }
}

SCENARIO("Construct a new const graph from an existing graph with the move constructor") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to construct a second graph using the move constructor and the first graph") {
      const gdwg::Graph<int, double> g2{std::move(g)};
      THEN("The resulting graph should contain the original graphs values, and the original graph "
           "should be empty") {
        std::vector<int> nodes1 = g.GetNodes();
        REQUIRE(nodes1.size() == 0);

        std::vector<int> nodes2 = g2.GetNodes();
        REQUIRE(nodes2.size() == 2);
        REQUIRE(nodes2[0] == 1);
        REQUIRE(nodes2[1] == 2);

        std::vector<double> edges2 = g2.GetWeights(1, 2);
        REQUIRE(edges2.size() == 3);
        REQUIRE(edges2[0] == 4.2);
        REQUIRE(edges2[1] == 5.2);
        REQUIRE(edges2[2] == 6.9);
      }
    }
  }
}

SCENARIO("Testing the copy assignment with a const graph") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    auto e1 = std::make_tuple(1, 2, 6.9);
    auto e2 = std::make_tuple(1, 2, 4.2);
    auto e3 = std::make_tuple(1, 2, 5.2);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to copy the original graph to a new graph using the copy assignment") {
      const gdwg::Graph<int, double> copy = g;
      THEN("The original and copy should contain the same values") {
        std::vector<int> nodes1 = g.GetNodes();
        REQUIRE(nodes1.size() == 2);
        REQUIRE(nodes1[0] == 1);
        REQUIRE(nodes1[1] == 2);

        std::vector<double> edges1 = g.GetWeights(1, 2);
        REQUIRE(edges1.size() == 3);
        REQUIRE(edges1[0] == 4.2);
        REQUIRE(edges1[1] == 5.2);
        REQUIRE(edges1[2] == 6.9);

        std::vector<int> nodes2 = copy.GetNodes();
        REQUIRE(nodes2.size() == 2);
        REQUIRE(nodes2[0] == 1);
        REQUIRE(nodes2[1] == 2);

        std::vector<double> edges2 = copy.GetWeights(1, 2);
        REQUIRE(edges2.size() == 3);
        REQUIRE(edges2[0] == 4.2);
        REQUIRE(edges2[1] == 5.2);
        REQUIRE(edges2[2] == 6.9);
      }
    }
  }
}

SCENARIO("Testing the move assignment by moving to a const graph") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    gdwg::Graph<int, double> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 6.9);
    g.InsertEdge(1, 2, 4.20);
    g.InsertEdge(1, 2, 5.2);
    WHEN("We try to move the original graph to a new graph using the move assignment") {
      const gdwg::Graph<int, double> move = std::move(g);
      THEN("The original should be empty and the new graph should contain the same values that the "
           "original used to") {
        std::vector<int> nodes1 = g.GetNodes();
        REQUIRE(nodes1.size() == 0);

        std::vector<int> nodes2 = move.GetNodes();
        REQUIRE(nodes2.size() == 2);
        REQUIRE(nodes2[0] == 1);
        REQUIRE(nodes2[1] == 2);

        std::vector<double> edges2 = move.GetWeights(1, 2);
        REQUIRE(edges2.size() == 3);
        REQUIRE(edges2[0] == 4.2);
        REQUIRE(edges2[1] == 5.2);
        REQUIRE(edges2[2] == 6.9);
      }
    }
  }
}

// Ensuring that duplicates get removed automatically in a const graph
SCENARIO("Testing that duplicate nodes and edges get removed in a const graph") {
  GIVEN("An const graph of strings and doubles with a duplicate node and edge") {
    std::string sydney{"sydney"};
    std::string melbourne{"melbourne"};
    std::string perth{"perth"};

    auto syd_melb2 = std::make_tuple(sydney, melbourne, 5.4);
    auto syd_melb = std::make_tuple(sydney, melbourne, 5.4);
    auto melb_per = std::make_tuple(melbourne, perth, 20.1);

    auto e =
        std::vector<std::tuple<std::string, std::string, double>>{syd_melb, melb_per, syd_melb2};
    const gdwg::Graph<std::string, double> g{e.begin(), e.end()};
    THEN("The resulting graph should automatically remove the duplicates") {
      std::vector<std::string> nodes = g.GetNodes();
      REQUIRE(nodes.size() == 3);
      REQUIRE(nodes[0] == melbourne);
      REQUIRE(nodes[1] == perth);
      REQUIRE(nodes[2] == sydney);
      std::vector<double> edges = g.GetWeights(sydney, melbourne);
      REQUIRE(edges.size() == 1);
      REQUIRE(edges[0] == 5.4);
    }
  }
}

SCENARIO("Testing the IsNode method with a const graph(successful case)") {
  GIVEN("A const graph of int nodes and double edges containing two nodes and an edge") {
    auto e1 = std::make_tuple(1, 2, 5.4);
    auto e2 = std::make_tuple(1, 2, 6.9);
    auto e3 = std::make_tuple(2, 3, 20.1);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to check the existence of an existing node using IsNode") {
      bool exist = g.IsNode(1);
      THEN("The return value should be true") { REQUIRE(exist == true); }
    }
  }
}

SCENARIO("Testing the IsNode method with a const graph (unsuccessful case)") {
  GIVEN("An graph of int nodes and double edges containing two nodes and an edge") {
    auto e1 = std::make_tuple(1, 2, 5.4);
    auto e2 = std::make_tuple(1, 2, 6.9);
    auto e3 = std::make_tuple(2, 3, 20.1);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to check the existence of an non-existent node using IsNode") {
      bool exist = g.IsNode(69);
      THEN("The return value should be false") { REQUIRE(exist == false); }
    }
  }
}

SCENARIO("Testing theIsConnected method with a const graph") {
  GIVEN("A const graph of int nodes and double edges containing two nodes and an edge") {
    auto e1 = std::make_tuple(1, 2, 5.4);
    auto e2 = std::make_tuple(1, 2, 6.9);
    auto e3 = std::make_tuple(2, 3, 20.1);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to check the existence of a connecting edge from the first node to the second "
         "using IsConnected") {
      bool exist = g.IsConnected(1, 2);
      THEN("The return value should be true") { REQUIRE(exist == true); }
    }
  }
}

SCENARIO("Testing the GetNodes method with a const graph to ensure correct nodes output in correct "
         "order") {
  GIVEN("A const graph of int nodes and double edges containing two nodes and an edge") {
    auto e1 = std::make_tuple(1, 2, 5.4);
    auto e2 = std::make_tuple(1, 2, 6.9);
    auto e3 = std::make_tuple(2, 3, 20.1);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to get a vector of nodes using GetNodes") {
      std::vector<int> nodes = g.GetNodes();
      THEN("The resulting vector should contain all the existing nodes in the correct order") {
        REQUIRE(nodes[0] == 1);
        REQUIRE(nodes[1] == 2);
        REQUIRE(nodes[2] == 3);
      }
    }
  }
}

SCENARIO("Testing the GetConnected method on a const graph to ensure correct nodes output in "
         "correct order") {
  GIVEN("A graph of int nodes and double edges containing some nodes and some edges") {
    auto e1 = std::make_tuple(1, 2, 5.4);
    auto e2 = std::make_tuple(1, 2, 6.9);
    auto e3 = std::make_tuple(1, 3, 20.1);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to get a vector of nodes connected to a src using GetConnected") {
      std::vector<int> connected = g.GetConnected(1);
      THEN("The resulting vector should contain all nodes connected to the source by an outgoing "
           "edge, sorted by increasing order") {
        REQUIRE(connected[0] == 2);
        REQUIRE(connected[1] == 3);
      }
    }
  }
}

SCENARIO("Testing the GetWeights method on a const graph to ensure correct weights output in "
         "correct order") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    auto e1 = std::make_tuple(1, 2, 5.4);
    auto e2 = std::make_tuple(1, 2, 6.9);
    auto e3 = std::make_tuple(1, 3, 20.1);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to get a vector of edge weights from a src to a dst using GetWeights") {
      std::vector<double> weights = g.GetWeights(1, 2);
      THEN("The resulting vector should contain edge weights from the src to the dst, sorted by "
           "increasing order") {
        REQUIRE(weights[0] == 5.4);
        REQUIRE(weights[1] == 6.9);
      }
    }
  }
}

SCENARIO("Testing the find method on a const graph with a valid edge") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    auto e1 = std::make_tuple(1, 2, 5.4);
    auto e2 = std::make_tuple(1, 2, 6.9);
    auto e3 = std::make_tuple(1, 3, 20.1);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to get an iterator to an edge found in the graph using the find method") {
      gdwg::Graph<int, double>::const_iterator it = g.find(1, 2, 6.9);
      THEN("The resulting iterator should point to a tuple containing the (src, dst, weight)") {
        REQUIRE(*(it) == std::tuple<int, int, double>(1, 2, 6.9));
      }
    }
  }
}

SCENARIO("Testing the find method on a const graph with an invalid edge") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    auto e1 = std::make_tuple(1, 2, 5.4);
    auto e2 = std::make_tuple(1, 2, 6.9);
    auto e3 = std::make_tuple(1, 3, 20.1);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to get an iterator to an edge found in the graph using the find method") {
      gdwg::Graph<int, double>::const_iterator it = g.find(1, 2, 7);
      THEN("The resulting iterator should point to cend") { REQUIRE(it == g.cend()); }
    }
  }
}

// only need to test one since begin returns cbegin
SCENARIO("Testing the begin and cbegin methods simultaneously on a const graph (successful case)") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    auto e2 = std::make_tuple(1, 2, 6.9);
    auto e3 = std::make_tuple(1, 2, 7);

    auto e = std::vector<std::tuple<int, int, double>>{e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to access the first element in the graph using the begin method") {
      gdwg::Graph<int, double>::const_iterator it = g.begin();
      THEN("The returned iterator should point to the first element in the graph") {
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 6.9));
      }
      THEN("We should be able to increment the returned iterator") {
        ++it;
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 7));
        THEN("We should be able to decrement the returned iterator back to its original position") {
          --it;
          REQUIRE(*it == std::tuple<int, int, double>(1, 2, 6.9));
        }
      }
    }
  }
}

// only need to test one since begin returns cbegin
SCENARIO("Testing the begin and cbegin methods simultaneously on an empty const graph") {
  GIVEN("An empty graph") {
    const gdwg::Graph<int, double> g;
    WHEN("We try to access the first element in the graph using the begin method") {
      gdwg::Graph<int, double>::const_iterator it = g.begin();
      THEN("Returned iterator should be equivalent to cbegin") { REQUIRE(it == g.cend()); }
    }
  }
}

// only need to test one since end returns cend
SCENARIO("Testing the end and cend methods simultaneously on a const graph (successful case)") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    auto e2 = std::make_tuple(1, 2, 6.9);
    auto e3 = std::make_tuple(1, 2, 7);

    auto e = std::vector<std::tuple<int, int, double>>{e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We get an iterator to the past-the-end element in the graph using the end method") {
      gdwg::Graph<int, double>::const_iterator it = g.end();
      THEN("We should be able to decrement the returned iterator so that it now points to the last "
           "element") {
        --it;
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 7));
        THEN("We should be able to increment the returned iterator back to its original position "
             "after decrementing it one more time") {
          --it;
          REQUIRE(*it == std::tuple<int, int, double>(1, 2, 6.9));
          ++it;
          REQUIRE(*it == std::tuple<int, int, double>(1, 2, 7));
        }
      }
    }
  }
}

// only need to test one since end returns cend
SCENARIO("Testing the end and cend methods simultaneously on an empty const graph") {
  GIVEN("An empty graph") {
    const gdwg::Graph<int, double> g;
    WHEN("We try to access the past-the-end element in the graph using the end method") {
      gdwg::Graph<int, double>::const_iterator it = g.end();
      THEN("Returned iterator should be equivalent to cbegin") { REQUIRE(it == g.cbegin()); }
    }
  }
}

// only need to test one since rbegin returns crbegin
SCENARIO(
    "Testing the crbegin and rbegin methods simultaneously on a const graph(successful case)") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    auto e2 = std::make_tuple(1, 2, 6.9);
    auto e3 = std::make_tuple(1, 2, 7);

    auto e = std::vector<std::tuple<int, int, double>>{e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to access the last element in the graph using the rbegin method") {
      gdwg::Graph<int, double>::const_reverse_iterator it = g.rbegin();
      THEN("The returned iterator should point to the last element in the graph") {
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 7));
      }
      THEN("We should be able to move the returned iterator backwards") {
        ++it;
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 6.9));
        THEN("We should be able to move the returned iterator forward to its original position") {
          --it;
          REQUIRE(*it == std::tuple<int, int, double>(1, 2, 7));
        }
      }
    }
  }
}

// only need to test one since crbegin returns rbegin
SCENARIO("Testing the crbegin and rbegin methods simultaneously on an empty const graph") {
  GIVEN("An empty graph") {
    const gdwg::Graph<int, double> g;
    WHEN("We try to access the last element in the graph using the rbegin method") {
      gdwg::Graph<int, double>::const_reverse_iterator it = g.rbegin();
      THEN("The iterator should be equivalent to crend") { REQUIRE(it == g.crend()); }
    }
  }
}

// only need to test one since crend returns rend
SCENARIO("Testing the crend and rend methods simultaneously on a const graph(successful case)") {
  GIVEN("A graph of int nodes and double edges containing two nodes and some edges") {
    auto e2 = std::make_tuple(1, 2, 6.9);
    auto e3 = std::make_tuple(1, 2, 7);

    auto e = std::vector<std::tuple<int, int, double>>{e3, e2};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to access the before-the-first element in the graph using the rend method") {
      gdwg::Graph<int, double>::const_reverse_iterator it = g.rend();
      THEN("We should be able to move the returned iterator forward to the first element") {
        --it;
        REQUIRE(*it == std::tuple<int, int, double>(1, 2, 6.9));
        THEN("We should be able to move the returned iterator backward to its original position "
             "after moving it forward one more time") {
          --it;
          REQUIRE(*it == std::tuple<int, int, double>(1, 2, 7));
          ++it;
          REQUIRE(*it == std::tuple<int, int, double>(1, 2, 6.9));
        }
      }
    }
  }
}

// only need to test one since crbegin returns rbegin
SCENARIO("Testing the crend and rend methods simultaneously on an empty const graph") {
  GIVEN("An empty graph") {
    const gdwg::Graph<int, double> g;
    WHEN("We try to access the before-the-first element in the graph using the rend method") {
      gdwg::Graph<int, double>::const_reverse_iterator it = g.rend();
      THEN("The iterator should be equivalent to crbegin") { REQUIRE(it == g.crbegin()); }
    }
  }
}

// TESTING FRIENDS STARTS HERE !!!!!!!!!!!!!!!! >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
SCENARIO("Testing the == friend operator on two equal const graphs") {
  GIVEN("Two graphs containing identical nodes and edges") {
    auto e1 = std::make_tuple(1, 2, 6.9);
    auto e2 = std::make_tuple(1, 3, 4.2);
    auto e3 = std::make_tuple(1, 4, 5.2);

    auto en1 = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g1{en1.begin(), en1.end()};

    auto eg1 = std::make_tuple(1, 2, 6.9);
    auto eg2 = std::make_tuple(1, 3, 4.2);
    auto eg3 = std::make_tuple(1, 4, 5.2);

    auto en2 = std::vector<std::tuple<int, int, double>>{eg1, eg2, eg3};
    const gdwg::Graph<int, double> g2{en2.begin(), en2.end()};
    WHEN("We try to use the == friend operator to see if they are equal") {
      bool equal = (g1 == g2);
      THEN("The result should equal true") { REQUIRE(equal == true); }
    }
  }
}

SCENARIO("Testing the == friend operator on two unequal const graphs") {
  GIVEN("Two graphs containing different nodes and edges") {
    auto e1 = std::make_tuple(1, 2, 6.9);
    auto e2 = std::make_tuple(1, 3, 4.2);
    auto e3 = std::make_tuple(1, 4, 5.2);

    auto en1 = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g1{en1.begin(), en1.end()};

    auto eg1 = std::make_tuple(1, 2, 7);
    auto eg2 = std::make_tuple(1, 3, 4.2);
    auto eg3 = std::make_tuple(1, 4, 5.2);

    auto en2 = std::vector<std::tuple<int, int, double>>{eg1, eg2, eg3};
    const gdwg::Graph<int, double> g2{en2.begin(), en2.end()};
    WHEN("We try to use the == friend operator to see if they are equal") {
      bool equal = (g1 == g2);
      THEN("The result should equal false") { REQUIRE(equal == false); }
    }
  }
}

SCENARIO("Testing the != friend operator on two equal const graphs") {
  GIVEN("Two graphs containing identical nodes and edges") {
    auto e1 = std::make_tuple(1, 2, 6.9);
    auto e2 = std::make_tuple(1, 3, 4.2);
    auto e3 = std::make_tuple(1, 4, 5.2);

    auto en1 = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g1{en1.begin(), en1.end()};

    auto eg1 = std::make_tuple(1, 2, 6.9);
    auto eg2 = std::make_tuple(1, 3, 4.2);
    auto eg3 = std::make_tuple(1, 4, 5.2);

    auto en2 = std::vector<std::tuple<int, int, double>>{eg1, eg2, eg3};
    const gdwg::Graph<int, double> g2{en2.begin(), en2.end()};
    WHEN("We try to use the != friend operator to see if they are equal") {
      bool equal = (g1 != g2);
      THEN("The result should equal false") { REQUIRE(equal == false); }
    }
  }
}

SCENARIO("Testing the != friend operator on two unequal const graphs") {
  GIVEN("Two graphs containing identical nodes and edges") {
    auto e1 = std::make_tuple(1, 2, 6.9);
    auto e2 = std::make_tuple(1, 3, 4.2);
    auto e3 = std::make_tuple(1, 4, 420.69);

    auto en1 = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g1{en1.begin(), en1.end()};

    auto eg1 = std::make_tuple(1, 2, 6.9);
    auto eg2 = std::make_tuple(1, 3, 4.2);
    auto eg3 = std::make_tuple(1, 4, 5.2);

    auto en2 = std::vector<std::tuple<int, int, double>>{eg1, eg2, eg3};
    const gdwg::Graph<int, double> g2{en2.begin(), en2.end()};
    WHEN("We try to use the != friend operator to see if they are equal") {
      bool equal = (g1 != g2);
      THEN("The result should equal true") { REQUIRE(equal == true); }
    }
  }
}

SCENARIO("Checking that the << friend operator works with a simple const graph") {
  GIVEN("A graph containing 2 int nodes and an edge connecting them") {
    auto e1 = std::make_tuple(1, 2, 6.9);
    auto e2 = std::make_tuple(2, 3, 1.1);
    auto e3 = std::make_tuple(3, 2, 1.2);
    auto e4 = std::make_tuple(3, 2, 1.4);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3, e4};
    const gdwg::Graph<int, double> g1{e.begin(), e.end()};
    WHEN("We try to use the << operator to print out the graph in order") {
      std::stringstream s;
      s << g1;
      THEN("The output stream should print the contents of the graph in the correct order") {
        std::string expected(
            "1 (\n  2 | 6.9\n)\n2 (\n  3 | 1.1\n)\n3 (\n  2 | 1.2\n  2 | 1.4\n)\n");
        REQUIRE(s.str() == expected);
      }
    }
  }
}

// EXCEPTION TESTING STARTS HERE
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

SCENARIO("Testing for exception in IsConnected when either node doesn't exist in const graph") {
  GIVEN("An graph of int nodes and double edges containing two nodes and some edges") {
    auto e1 = std::make_tuple(1, 2, 6.9);
    auto e2 = std::make_tuple(1, 2, 4.2);
    auto e3 = std::make_tuple(1, 2, 5.2);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to use the IsConnected method from a non existent node to an existing one") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.IsConnected(3, 1),
            "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
      }
    }
    WHEN("We try to use the IsConnected method from an existing node to a non existent one") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.IsConnected(1, 3),
            "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
      }
    }
    WHEN("We try to use the IsConnected method between two non existent nodes") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.IsConnected(69, 3),
            "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
      }
    }
  }
}

SCENARIO("Testing for exception in GetConnected when the src node doesn't exist in const graph") {
  GIVEN("An graph of int nodes and double edges containing two nodes and some edges") {
    auto e1 = std::make_tuple(1, 2, 6.9);
    auto e2 = std::make_tuple(1, 2, 4.2);
    auto e3 = std::make_tuple(1, 2, 5.2);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to use the GetConnected method with a non existent src node") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(g.GetConnected(3),
                            "Cannot call Graph::GetConnected if src doesn't exist in the graph");
      }
    }
  }
}

SCENARIO(
    "Testing for exception in GetWeights when the src or dst node doesn't exist in const graph") {
  GIVEN("An graph of int nodes and double edges containing two nodes and some edges") {
    auto e1 = std::make_tuple(1, 2, 6.9);
    auto e2 = std::make_tuple(1, 2, 4.2);
    auto e3 = std::make_tuple(1, 2, 5.2);

    auto e = std::vector<std::tuple<int, int, double>>{e1, e2, e3};
    const gdwg::Graph<int, double> g{e.begin(), e.end()};
    WHEN("We try to use the GetWeights method with a non existent src node") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.GetWeights(3, 1),
            "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
      }
    }
    WHEN("We try to use the GetWeights method with a non existent dst node") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.GetWeights(1, 3),
            "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
      }
    }
    WHEN("We try to use the GetWeights method with a non existent src and dst node") {
      THEN("We should throw the correct exception") {
        REQUIRE_THROWS_WITH(
            g.GetWeights(3, 69),
            "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
      }
    }
  }
}
