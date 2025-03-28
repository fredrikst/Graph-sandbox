module;

#include <functional>
#include <chrono>
#include <queue>
#include <unordered_set>
#include <thread>

export module BreadthFirst;
import NavGraph;
import NavGraphRenderer;

using namespace std::chrono_literals;

export class BreadthFirst {
public:
    BreadthFirst(const NavGraph& navGraph, NavGraphRenderer& renderer)
    : navGraph_(navGraph), renderer_(renderer) {}

    void search(std::function<bool(Node*)> endFunction) {
        auto startNode = navGraph_.getStartNode();

        std::queue<Node*> nodesToVisit;
        std::unordered_set<const Node*> encounteredNodes;
        nodesToVisit.push(startNode);
        encounteredNodes.insert(startNode);
        

        while (!nodesToVisit.empty()) {
            auto currentNode = nodesToVisit.front();
            nodesToVisit.pop();

            auto isNodeGoal = endFunction(currentNode);

            renderer_.setVisitedNode(currentNode);

            if (isNodeGoal) {
                break;
            }

            std::this_thread::sleep_for(1000ms);

            auto neighbors = currentNode->getNeighbors();

            for (const auto& neighbor : neighbors) {
                auto itFound = encounteredNodes.find(neighbor.first);

                if (itFound == encounteredNodes.end()) {
                    nodesToVisit.push(neighbor.first);
                    encounteredNodes.insert(neighbor.first);
                }
            }
        }
    }
private:
    const NavGraph& navGraph_;
    NavGraphRenderer& renderer_;
};
