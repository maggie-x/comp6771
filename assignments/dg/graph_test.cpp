/*

  == Explanation and rational of testing ==
  The scope of our tests are mainly concerned with ensuring the correctness of each function
  (constructor, overload, method) that we implemented. To do so we created brief tests for each
  function (since each part of the program we implemented is relatively simple, if it works for a
  simple case, it should work for more complicated cases too).

  First, we will test all the constructors and ensure that they work. From there, we just test each method/operator on
  any graph, since once the graph is constructed, it will behave the same with the methods/operators no matter how it was
  initially constructed.

  Then, we will test edge cases such as calling certain methods/operators on an empty graph, using non-primitive types, and iterating past
  the end of the graph.

  Furthermore, we will also test each exception to check that it is throwing the correct error and string.

  Each feature should be tested with it's own individual scenario to ensure that it's clear what is being tested
  and what could have gone wrong.

DONE:
[x] change E to a smart pointer
[x] sort the edges in a smarter way? instead of doing it before we return
[ ] HOW TO TEST individual methods when we don't have access to private fields
    to check if the operation was actually successful
[x] reverse iterator 
- (automatic decrement or we have to implement ourselves?)
- returning reference in post-increment/decrement
- do we need a pointer type for our iterator?

TODO:
[ ] more efficient method for bool operator==?
[ ] more thorough tests
- using non-primitive types
- self-connected edges

[ ] check correct errors are being thrown & return types are correct
[ ] const correctness
[ ] clean up some code
[ ] edge cases
[ ] exception throwing tests




*/



#include "assignments/dg/graph.h"
#include "catch.h"

#include <algorithm>

template<typename T>
bool isEqual(std::vector<T> const &v1, std::vector<T> const &v2)
{
    return (v1.size() == v2.size() &&
            std::equal(v1.begin(), v1.end(), v2.begin()));
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
        THEN("The graph should be successfully constructed and contain the correct nodes in the correct order") {
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
        gdwg::Graph<std::string, double> graph{"red", "orange", "yellow", "green", "blue", "indigo", "violet"};
        THEN("The graph should be successfully constructed and contain the correct nodes and edges") {
            std::vector<std::string> colours{"blue", "green", "indigo", "orange", "red", "violet", "yellow" };
            REQUIRE(isEqual(graph.GetNodes(), colours));
        }
    }
}

/*SCENARIO("Construct a new graph from an existing graph with the copy constructor") {
  std::string sydney{"sydney"};
  std::string melbourne{"melbourne"};
  std::string perth{"perth"};

  auto syd_melb = std::make_tuple(sydney, melbourne, 5.4);
  auto melb_per = std::make_tuple(melbourne, perth, 20.1);

  auto e = std::vector<std::tuple<std::string, std::string, double>>{syd_melb, melb_per};
  const gdwg::Graph<std::string, double> aus{e.begin(), e.end()};

  THEN("when we construct a new graph from this graph") {
    gdwg::Graph<std::string, double> aus2{aus};

    REQUIRE(aus == aus2);
  }
}*/

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

SCENARIO("Testing the InsertEdge method") {
    GIVEN("An graph of int nodes and double edges containing just two nodes") {
        gdwg::Graph<int, double> g;
        g.InsertNode(1);
        g.InsertNode(2);
        WHEN("We try to insert an edge between from one node to the other using the InsertEdge method") {
            g.InsertEdge(1, 2, 2.5);
            THEN("We should get a graph which contains those two nodes and an edge from the first to the second") {
                std::vector<int> nodes = g.GetNodes();
                REQUIRE(nodes.size() == 2);
                REQUIRE(nodes[0] == 1);
                REQUIRE(nodes[1] == 2);

                std::vector<double> edges = g.GetWeights(1,2);
                REQUIRE(edges[0] == 2.5);
            }
        }

        // TODO CHECK A NODE CONNECTING TO ITSELF
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
            THEN("The value of the first node should now be set to a different one by the replace method") {
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
        WHEN("We try to replace the first node with an already existing value using the Replace method") {
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
    GIVEN("A graph of strings and doubles") {
        std::string sydney{"sydney"};
        std::string melbourne{"melbourne"};
        std::string perth{"perth"};
        std::string adelaide{"adelaide"};
        std::string brisbane{"brisbane"};

        auto syd_melb = std::make_tuple(sydney, melbourne, 5.4);
        auto syd_syd = std::make_tuple(sydney, sydney, 4.2);
        auto mel_mel = std::make_tuple(melbourne, melbourne, 5.4);
        auto per_syd = std::make_tuple(perth, sydney, 6.9);
        auto per_mel = std::make_tuple(perth, melbourne, 6.9);
        auto melb_per = std::make_tuple(melbourne, perth, 20.1);
        auto per_ade = std::make_tuple(perth, adelaide, 25.9);
        auto syd_ade = std::make_tuple(sydney, adelaide, 4.7);
        auto ade_bris = std::make_tuple(adelaide, brisbane, 2.3);

        auto e = std::vector<std::tuple<std::string, std::string, double>>{syd_syd, syd_melb, melb_per, per_ade, syd_ade, ade_bris, per_syd, mel_mel, per_mel};
        gdwg::Graph<std::string, double> aus{e.begin(), e.end()};
        WHEN("We try to replace and merge the old node with an already existing value using the MergeReplace method") {
            aus.MergeReplace(sydney, melbourne);
            THEN("The graph should no longer contain the old node, all outgoing edges from the old node should now be outgoing from the"
                 "new node, and all incoming edges to the old node should now be incoming to the new node") {
                std::vector<std::string> nodes = aus.GetNodes();
                REQUIRE(nodes.size() == 4); // ensure that the old node has been deleted
                REQUIRE(nodes[0] == adelaide);
                REQUIRE(nodes[1] == brisbane);
                REQUIRE(nodes[2] == melbourne);
                REQUIRE(nodes[3] == perth);

                std::vector<double> mel_per_weights = aus.GetWeights(melbourne, perth);
                std::vector<double> mel_mel_weights = aus.GetWeights(melbourne, melbourne);
                std::vector<double> mel_ade_weights = aus.GetWeights(melbourne, adelaide);
                std::vector<double> per_mel_weights = aus.GetWeights(perth, melbourne);
                REQUIRE(mel_per_weights[0] == 20.1);
                REQUIRE(mel_mel_weights[0] == 4.2);
                REQUIRE(mel_mel_weights[1] == 5.4);
                REQUIRE(mel_ade_weights[0] == 4.7);
                REQUIRE(per_mel_weights[0] == 6.9);
                REQUIRE(mel_mel_weights.size() == 2); // there should still be only one edge from melbourne to melbourne since the duplicate should be removed
                REQUIRE(per_mel_weights.size() == 1);
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
  // std::cout << graph;

  // since the nodes will be sorted
  std::vector<std::string> colours{"blue", "green", "indigo", "orange", "red", "violet", "yellow" };
  REQUIRE(isEqual(graph.GetNodes(), colours));

  // TESTING FOR CORRECT ORDERING OF EDGES IN NODE
  WHEN("multiple nodes are added as outgoing nodes to one node") {
    graph.InsertEdge("violet", "red", 0.8);
    graph.InsertEdge("violet", "orange", 0.7);
    graph.InsertEdge("violet", "yellow", 0.6);
    graph.InsertEdge("violet", "green", 0.5);
    graph.InsertEdge("violet", "blue", 0.4);
    graph.InsertEdge("violet", "indigo", 0.3);
    graph.InsertEdge("violet", "violet", 0.2);

    THEN("the edges should be ordered by their destination node") {
      std::vector<std::string> expected{"blue", "green", "indigo", "orange", "red", "violet", "yellow"};
      REQUIRE(expected == graph.GetConnected("violet"));
    }
  }

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
    // std::cout << graph;

    THEN("the edges should be ordered by their weight to the destination node") {
      std::vector<double> expected{0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
      REQUIRE(expected == graph.GetWeights("violet", "yellow"));
    }
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

    // std::cout << graph;

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
    // std::cout << graph;
  }

  WHEN("an edge is deleted") {
    // std::cout << graph;
    // std::cout << "deleting edge hello - 0.99 - string" << std::endl;
    REQUIRE(graph.IsConnected("hello", "string"));
    REQUIRE(graph.erase("hello", "string", 0.99));
    REQUIRE(!graph.IsConnected("hello", "string"));
    // std::cout << graph;
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
    
    // std::cout << aus;
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
    // std::cout << "-- TESTING CUSTOM ITERATOR --" << std::endl;
    // std::cout << aus;
    // for (auto it = aus.cbegin(); it != aus.cend(); ++it){
    //   std::cout << "<" << std::get<0>(*it) << ", " << std::get<1>(*it) << ", " << std::get<2>(*it) << ">" << std::endl;
    // }

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

    // std::cout << "-- TESTING REVERSE ITERATOR --" << std::endl;
    // for (auto rit = aus.rbegin(); rit != aus.rend(); ++rit) {
    //   std::cout << "<" << std::get<0>(*(rit)) << ", " << std::get<1>(*rit) << ", " << std::get<2>(*rit) << ">" << std::endl;
    // }

    std::vector<std::tuple<std::string, std::string, double>> reverse_edges(aus.crbegin(), aus.crend());
    auto t1 = std::make_tuple("adelaide", "brisbane", 2.3);
    auto t2 = std::make_tuple("melbourne", "perth", 20.1);
    auto t3 = std::make_tuple("perth", "adelaide", 25.9);
    auto t4 = std::make_tuple("sydney", "adelaide", 4.7);
    auto t5 = std::make_tuple("sydney", "melbourne", 5.4);
    std::vector<std::tuple<std::string, std::string, double>> expected{t5, t4, t3, t2, t1};
    REQUIRE(reverse_edges == expected);

  }
  

  WHEN("We use find to get an iterator to a particular edge in our graph") {
      auto it = aus.find("perth", "adelaide", 25.9);

      auto t2 = std::make_tuple("melbourne", "perth", 20.1);
      auto t3 = std::make_tuple("perth", "adelaide", 25.9);
      auto t4 = std::make_tuple("sydney", "adelaide", 4.7);

      REQUIRE(*it == t3);

      WHEN("incremented forward") {
          std::advance(it, 1);
          REQUIRE(*it == t4);
      }

      WHEN("decremented backward") {
          std::advance(it, -1);
          REQUIRE(*it == t2);
      }

  }

  WHEN("we use find to get an edge that doesn't exist") {
    // adelaide is now plural, so shouldn't show up
    REQUIRE(aus.find("perth", "adelaides", 25.9) == aus.end());
  }

  WHEN("we erase via an iterator on the graph") {
    // std::cout << aus;
    auto it = aus.find("perth", "adelaide", 25.9);
    auto new_it = aus.erase(it);
    REQUIRE(!aus.IsConnected("perth", "adelaide"));
    // std::cout << aus;

    auto after = std::make_tuple("sydney", "adelaide", 4.7);
    REQUIRE(*new_it == after);
    // std::cout << "new it after erasing <" << std::get<0>(*new_it) << ", " << std::get<1>(*new_it) << ", " << std::get<2>(*new_it) << ">" << std::endl;

    WHEN("we erase by passing in the end, we should get back the end") {
      REQUIRE(aus.erase(aus.end()) == aus.end());
    }

  }

  WHEN("We try to use the MergeReplace function on it") {
      aus.MergeReplace(sydney, melbourne);
      THEN("We should get a graph where sydney edges are replaced by melbourne edges") {
            REQUIRE(aus.IsConnected("melbourne", "adelaide"));
            REQUIRE(aus.IsConnected("melbourne", "melbourne"));
            REQUIRE(aus.IsConnected("melbourne", "perth"));
            REQUIRE(!(aus.IsNode("sydney")));
      }
  }

}