#include "graph.h"

PriorityQueue::PriorityQueue() : front(nullptr) {}

bool PriorityQueue::isEmpty() const {
    return front == nullptr;
}

void PriorityQueue::enqueue(Vertex* v) {
    ListNode* newNode = new ListNode(v);
    if (isEmpty() || v->dist < front->element->dist) {
        newNode->next = front;
        front = newNode;
    } else {
        ListNode* current = front;
        while (current->next != nullptr && current->next->element->dist <= v->dist) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

Vertex* PriorityQueue::dequeue() {
    if (isEmpty()) {
        cerr << "PriorityQueue is empty." << endl;
        exit(0);
    }
    Vertex* frontItem = front->element;
    ListNode* old = front;
    front = front->next;
    delete old;
    return frontItem;
}


void Graph::printGraph() {
    Vertex* v = head;
    while (v) {
        for (Edge* e = v->adj; e; e = e->next) {
            Vertex* w = e->des;
            cout << v->name << " -> " << w->name << "  " << e->cost << "   " << (w->dist == INFINITY ? "inf" : to_string(w->dist)) << endl;
        }
        v = v->next;
    }
}

Vertex* Graph::getVertex(const char vname) {
    Vertex* ptr = head;
    Vertex* newv;
    if (ptr == nullptr) {
        newv = new Vertex(vname);
        head = newv;
        tail = newv;
        numVertices++;
        return newv;
    }
    while (ptr) {
        if (ptr->name == vname)
            return ptr;
        ptr = ptr->next;
    }
    newv = new Vertex(vname);
    tail->next = newv;
    tail = newv;
    numVertices++;
    return newv;
}

void Graph::addEdge(const char sn, const char dn, double c) {
    Vertex* v = getVertex(sn);
    Vertex* w = getVertex(dn);
    Edge* newEdge = new Edge(w, c);
    newEdge->next = v->adj;
    v->adj = newEdge;
    (v->numEdges)++;
    // point 1
}

void Graph::clearAll() {
    Vertex* ptr = head;
    while (ptr) {
        ptr->reset();
        ptr = ptr->next;
    }
}

void Graph::dijkstra(const char start) {
// STUDENTS MUST COMPLETE THE DEFINITION OF THIS FUNCTION
// chatgpt assisted a couple of lines of code to get me started.

    clearAll(); // reset data


    Vertex* s = getVertex(start); // point s to start vertex

    if (!s){ // return if DNE
        return;
    }

    s->dist = 0; // set distance to zero

    PriorityQueue pq; // create queue
    pq.enqueue(s); // enqueue current vertex

    while (!pq.isEmpty()) { // while there are vertices to visit, continue the following code
        Vertex* v = pq.dequeue();
        if (v->scratch) continue; // already been visited
        v->scratch = 1;

        for (Edge* e = v->adj; e != nullptr; e = e->next) { // iterate through all neighbours of v
            Vertex* w = e->des;
            double newDist = v->dist + e->cost;
            if (w->dist > newDist) {

                w->dist = newDist;
                w->prev = v;
                pq.enqueue(w);

            }

        }
    }
}

void Graph::unweighted(const char start) {
// STUDENTS MUST COMPLETE THE DEFINITION OF THIS FUNCTION
// a lot of logic copied from Dijkstra's solution.

    clearAll(); // Reset all data

    Vertex* s = getVertex(start); // point s to start vertex

    if (!s){ // if s DNE, return
        return;
    }


    s->dist = 0; // set distance to 0

    queue<Vertex*> q; // new queue
    q.push(s); // add s to queue

    while (!q.empty()) { // check all edges connected to current vertex
        Vertex* v = q.front(); q.pop();

        for (Edge* e = v->adj; e != nullptr; e = e->next) {
            Vertex* w = e->des;

            if (w->dist == INFINITY) {

                w->dist = v->dist + 1;
                w->prev = v;
                q.push(w);

            }

        }

    }
}

void Graph::readFromFile(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    char sn, dn;
    double cost;
    while (infile >> sn >> dn >> cost) {
        addEdge(sn, dn, cost);
    }

    infile.close();
}

void Graph::printPath(Vertex* dest) {
    if (dest->prev != nullptr) {
        printPath(dest->prev);
        cout << " " << dest->name;
    } else {
        cout << dest->name;
    }
}

void Graph::printAllShortestPaths(const char start, bool weighted) {
    if (weighted) {
        dijkstra(start);
    } else {
        unweighted(start);
    }
    setiosflags(ios::fixed);
    setprecision(2);
    Vertex* v = head;
    while (v) {
        if (v->name == start) {
            cout << start << " -> " << v->name << "     0   " << start << endl;
        } else {
            
            cout << start << " -> " << v->name << "     " << (v->dist == INFINITY ? "inf" : to_string((int)v->dist)) << "   ";
            if (v->dist == INFINITY) {
                cout << "No path" << endl;
            } else {
                printPath(v);
                cout << endl;
            }
        }
        v = v->next;
    }
}
