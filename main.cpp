#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <math.h>
// use std::

struct Node {
    std::vector<int> puzzle;
    std::string path = "";
    int misplacedTileHeuristic = 0;
    int manhattanDistanceHeuristic = 0; 

    unsigned size = 0;
    int blankIndex = -1;

    Node(std::vector<int> puzzle) : puzzle(puzzle), size(sqrt(puzzle.size())) {
        for (unsigned i = 0; i < puzzle.size(); i++) {
            if (puzzle.at(i) == 0) {
                blankIndex = i;
            }
        }
    }
    
    // Evaluates number of misplaced tiles in the puzzle not including blank
    void setMisplacedTileHeuristic() {
        for (unsigned i = 0; i < puzzle.size(); i++) {
            if (puzzle.at(i) != 0 && puzzle.at(i) != i + 1) misplacedTileHeuristic++;
        }
    }

    // Evaluates Manhattan distance of the puzzle not including blank
    void setManhattanDistanceHeuristic() {
        for (unsigned i = 0; i < puzzle.size(); i++) {
            if (puzzle.at(i) != 0 && puzzle.at(i) != i + 1) {
                int correctPosition = puzzle.at(i) - 1;
                int xDistance = correctPosition % size - (i % size); // FIXME: abs - overloaded for unsigned b/c of modulo
                int yDistance = (i / size) - (correctPosition / size);
                manhattanDistanceHeuristic += std::abs(xDistance) + std::abs(yDistance);
            }
        }
    }

    // Returns visual representation of the node
    std::string getTextNode() const {
        std::string textNode = "";
        for (unsigned i = 0; i < puzzle.size(); i++) {
            textNode += std::to_string(puzzle.at(i)) + ' ';
            if ((i + 1) % size == 0) {
                textNode += '\n';
            }
        }

        return textNode;
    }

    void printNode() const {
        std::string textNode = getTextNode();
        std::cout << textNode;
    }

    void printMoves() const {
        for (unsigned i = 0; i < path.size(); i++) {
            if (path.at(i) == 'L') {
                std::cout << "Left ";
            }
            else if (path.at(i) == 'R') {
                std::cout << "Right ";
            }
            else if (path.at(i) == 'U') {
                std::cout << "Up ";
            }
            else if (path.at(i) == 'D') {
                std::cout << "Down ";
            }
        }
        std::cout << std::endl;
    }

    void printPath() const {
        // Find the path that contains reverse of each move
        printMoves();

        std::string matchedPath = "";
        for (unsigned i = 0; i < path.size(); i++) {
            if (path.at(i) == 'L') {
                matchedPath += 'R';
            }
            else if (path.at(i) == 'R') {
                matchedPath += 'L';
            }
            else if (path.at(i) == 'U') {
                matchedPath += 'D';
            }
            else if (path.at(i) == 'D') {
                matchedPath += 'U';
            }
            else { // Error case
                return;
            }
        }

        std::string textPath = ""; // string - contains the visual representation of the path
        std::stack<Node*> nodesPath; // queue - contains the Nodes of the path 

        // FIXME: How do I push 'this' pointer to my queue?
        // nodesPath.push(this);

        // Add current node as the first item in stack
        Node* newNode = nullptr;
        newNode = new Node(this->puzzle);
        nodesPath.push(newNode);

        for (int i = matchedPath.size() - 1; i >= 0; i--) {
            if (matchedPath.at(i) == 'L') {
                newNode = nodesPath.top()->moveLeft();
            } 
            else if (matchedPath.at(i) == 'R') {
                newNode = nodesPath.top()->moveRight();
            } 
            else if (matchedPath.at(i) == 'U') {
                newNode = nodesPath.top()->moveUp();
            } 
            else if (matchedPath.at(i) == 'D') {
                newNode = nodesPath.top()->moveDown();
            } 
            else {
                break;
            }

            // Check for error
            if (newNode == nullptr) {
                std::cout << "error";
                return;
            }
            
            nodesPath.push(newNode);
        }
        
        while(!nodesPath.empty()) {
            textPath += nodesPath.top()->getTextNode() + '\n';
            nodesPath.pop();
        }
        std::cout << textPath;
    }

    bool goalTest() const {
        for (unsigned i = 0; i < puzzle.size() - 1; i++) { //FIXME?
            if (puzzle.at(i) != i + 1) return false;
        }
        
        if (puzzle.at(puzzle.size() - 1) == 0) {
            return true;
        }
        else { // puzzle.at(puzzle.size()) != 0
            return false;
        }
    }

    Node* moveLeft() const {
        // are we checking for repeated states to pruning? how would we even do this?
        if ((blankIndex) % size == 0) {
            return nullptr;
        }
        else {
            std::vector<int> newPuzzle = puzzle;
            newPuzzle.at(blankIndex) = newPuzzle.at(blankIndex - 1);
            newPuzzle.at(blankIndex - 1) = 0;

            Node* newNode = new Node(newPuzzle);
            newNode->path += this->path + 'L';
            return newNode;
        }
        
    }

    Node* moveRight() const {
        // are we checking for repeated states to pruning? how would we even do this?
        if ((blankIndex) % size == size - 1) {
            return nullptr;
        }
        else {
            std::vector<int> newPuzzle = puzzle;
            newPuzzle.at(blankIndex) = newPuzzle.at(blankIndex + 1);
            newPuzzle.at(blankIndex + 1) = 0;

            Node* newNode = new Node(newPuzzle);
            newNode->path += this->path + 'R';
            return newNode;
        }
    }
    
    Node* moveUp() const {
        // are we checking for repeated states to pruning? how would we even do this?
        if ((blankIndex) < size) {
            return nullptr;
        }
        else {
            std::vector<int> newPuzzle = puzzle;
            newPuzzle.at(blankIndex) = newPuzzle.at(blankIndex - size);
            newPuzzle.at(blankIndex - size) = 0;

            Node* newNode = new Node(newPuzzle);
            newNode->path += this->path + 'U';
            return newNode;
        }
    }

    Node* moveDown() const {
        // are we checking for repeated states to pruning? how would we even do this?
        if ((blankIndex) >= (size * (size - 1))) {
            return nullptr;
        }
        else {
            std::vector<int> newPuzzle = puzzle;
            newPuzzle.at(blankIndex) = newPuzzle.at(blankIndex + size);
            newPuzzle.at(blankIndex + size) = 0;

            Node* newNode = new Node(newPuzzle);
            newNode->path += this->path + 'D';
            return newNode;
        }
    }

    // int getValue(int x, int y) {
    //     // 
    //     //
    // }
};

std::vector<Node*> expand(Node* node) {
    std::vector<Node*> children;
    children.push_back(node->moveLeft());
    children.push_back(node->moveRight());
    children.push_back(node->moveUp());
    children.push_back(node->moveDown());

    return children;
}

void queueingFunction(std::queue<Node*> &nodes, std::vector<Node*> expandedNodes) {
    for (unsigned i = 0; i < expandedNodes.size(); i++) {
        if (expandedNodes.at(i) != nullptr) {
            nodes.push(expandedNodes.at(i));
        }
    }
}

Node* uniformCostSearch(Node* problem, int heuristic) {
    // do i need to free pointers?
    std::queue<Node*> nodes;
    nodes.push(problem);

    while (!nodes.empty()) {
        Node* node = nodes.front();
        nodes.pop();

        if (node->goalTest()) {
            return node;
        }

        queueingFunction(nodes, expand(node));
    }

    Node* failure = nullptr;
    return failure;
}

int main() {
    std::vector<int> test = {1,2,3,5,0,6,4,7,8};
    Node* n = new Node(test);
    // n->printNode();  
    // std::cout << std::endl;
    // n->setMisplacedTileHeuristic();
    // n->setManhattanDistanceHeuristic();
    // std::cout << n->misplacedTileHeuristic << std::endl;
    // std::cout << n->manhattanDistanceHeuristic << std::endl;

    Node* solution = uniformCostSearch(n, 0);
    if (solution == nullptr) 
        std::cout << "failure" << std::endl;
    else
        std::cout << "Solution:" << std::endl;
        solution->printPath();
        std::cout << std::endl;
}