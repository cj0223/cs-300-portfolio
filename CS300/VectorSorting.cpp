//============================================================================
// Name        : VectorSorting.cpp
// Author      : Curtis Harvey
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Vector Sorting Algorithms
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Static methods used for testing
//============================================================================

void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
}

Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

vector<Bid> loadBids(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    vector<Bid> bids;
    csv::Parser file = csv::Parser(csvPath);

    try {
        for (int i = 0; i < file.rowCount(); i++) {
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');
            bids.push_back(bid);
        }
    }
    catch (csv::Error& e) {
        cerr << e.what() << endl;
    }

    return bids;
}

//============================================================================
// Sorting Algorithms
//============================================================================

void selectionSort(vector<Bid>& bids) {

    for (size_t pos = 0; pos < bids.size() - 1; ++pos) {

        size_t minIndex = pos;

        for (size_t j = pos + 1; j < bids.size(); ++j) {
            if (bids[j].title < bids[minIndex].title) {
                minIndex = j;
            }
        }

        if (minIndex != pos) {
            swap(bids[pos], bids[minIndex]);
        }
    }
}

int partition(vector<Bid>& bids, int begin, int end) {

    string pivot = bids[end].title;
    int low = begin - 1;

    for (int high = begin; high < end; ++high) {
        if (bids[high].title <= pivot) {
            ++low;
            swap(bids[low], bids[high]);
        }
    }

    swap(bids[low + 1], bids[end]);
    return low + 1;
}

void quickSort(vector<Bid>& bids, int begin, int end) {

    if (begin >= end) {
        return;
    }

    int mid = partition(bids, begin, end);

    quickSort(bids, begin, mid - 1);
    quickSort(bids, mid + 1, end);
}

//============================================================================
// Utility
//============================================================================

double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

//============================================================================
// Main
//============================================================================

int main(int argc, char* argv[]) {

    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
    }

    vector<Bid> bids;
    clock_t ticks;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. Quick Sort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            ticks = clock();
            bids = loadBids(csvPath);
            cout << bids.size() << " bids read" << endl;
            ticks = clock() - ticks;
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            for (int i = 0; i < bids.size(); ++i) {
                displayBid(bids[i]);
            }
            cout << endl;
            break;

        case 3:
            ticks = clock();
            selectionSort(bids);
            ticks = clock() - ticks;
            cout << bids.size() << " bids sorted" << endl;
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            ticks = clock();
            quickSort(bids, 0, bids.size() - 1);
            ticks = clock() - ticks;
            cout << bids.size() << " bids sorted" << endl;
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}