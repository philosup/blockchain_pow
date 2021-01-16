#include <iostream>

#include "blockchain/block.hpp"
#include "blockchain/blockchain.hpp"
#include "blockchain/uint256.h"
#include "httplib.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/reader.h"

using namespace std;

bool checkPort(int port);
void addBlock(int port, Block& block);
void mine(int port, int difficulty, int begin, int end);

int port = 8080;
Blockchain chain;

vector<int> node_ports;


int main(int argc, char** argv){

    // if(argc != 2){
    //     cout<< "need argument to port !!" << endl;
    //     return -1;
    // }

    // int port = strtol(argv[1], NULL, 10);

    while(checkPort(port) == true) {
        node_ports.push_back(port++);
    }
    cout<< "Start !!" << endl;

    // HTTP
    httplib::Server svr;

    svr.Get("/hi", [](const httplib::Request &req, httplib::Response &res) {
        res.set_content("Hello World! : " + to_string(port), "text/plain");
    });

    svr.Post("/node", [](const httplib::Request &req, httplib::Response &res) {

        Document d;
        d.Parse(req.body);

        if(d.HasParseError()){
            res.set_content("invalid json format", "text/plain");
            return;
        }

        if(d.HasMember("port") == false){
            res.set_content("required 'port' field", "text/plain");
            return;
        }

        int node_port = d["port"].GetInt();

        node_ports.push_back(node_port);
        cout<<"add port : "<< node_port <<endl;
        res.set_content("add port : " + to_string(node_port), "text/plain");
    });


    svr.Post("/mine", [](const httplib::Request &req, httplib::Response &res) {
        int diff = 16;
        long begin = 0;
        long end = 1000'0000;

        Document d;
        d.Parse(req.body);

        if(d.HasParseError() == false)
        {
            if(d.HasMember("difficulty")){
                Value& v = d["difficulty"];
                diff = v.GetInt();
                cout<<"set difficulty : "<< diff <<endl;
            }
            else
                cout<<"difficulty is not a member"<<endl;

            if(d.HasMember("begin") && d.HasMember("end")){
                begin = d["begin"].GetInt64();
                end = d["end"].GetInt64();
            }
            else{
                cout<<"properties(begin/end) are not setted"<<endl;
            }
        }

        auto block = chain.getLastBlock();
        cout<< block.GetJSON() << endl;

        uint256 UINT256_MAX = 0;
        UINT256_MAX--;

        uint256 difficult = UINT256_MAX >>= diff;

        auto prevhash = block.hash;
        auto index = block.index + 1;
        long timestamp = diff;
        for(long i = begin; i< end; i++){
            auto block2 = Block(index, timestamp, prevhash, i);
            auto hash = calculateHash(&block2);

            if( hash < difficult){
                // cout<< block2.GetJSON() << endl;
                prevhash = block2.hash;

                addBlock(port, block2);
                //TODO: broadcast to other nodes
                for(auto nPort : node_ports){
                    addBlock(nPort, block2);
                }
                break;
            }
            if(chain.getLastBlock().index != index -1)
            {
                break;
            }
        }
        res.set_content(chain.GetJSON(), "application/json");
    });


    svr.Post("/mine_all", [](const httplib::Request &req, httplib::Response &res) {
        int diff = 16;
        long begin = 0;
        long end = 1000'0000;

        Document d;
        d.Parse(req.body);

        if(d.HasParseError() == false)
        {
            if(d.HasMember("difficulty")){
                Value& v = d["difficulty"];
                diff = v.GetInt();
                cout<<"set difficulty : "<< diff <<endl;
            }
            else
                cout<<"difficulty is not a member"<<endl;

            if(d.HasMember("begin") && d.HasMember("end")){
                begin = d["begin"].GetInt64();
                end = d["end"].GetInt64();
            }
            else{
                cout<<"properties(begin/end) are not setted"<<endl;
            }
        }


        thread t(mine, port, diff, begin, end);
        t.detach();
        auto range = end - begin;
        for(auto nPort : node_ports){
            begin = end;
            end = begin + range;
            thread t(mine, port, diff, begin, end);
            t.detach();
        }

        res.set_content("start mining !!!","text/plain");
    });

    svr.Post("/block", [](const httplib::Request &req, httplib::Response &res) {
        cout<<req.body<<endl;
        auto block = Block(req.body);

        chain.addBlock(block);
        //todo: validation
        res.set_content(block.GetJSON(), "application/json");
    });

    svr.Post("/exit", [](const httplib::Request &req, httplib::Response &res) {
        res.set_content("exit", "text/plain");
        exit(0);
    });


    for(auto nPort : node_ports){
        httplib::Client cli("localhost", nPort);

        char param[100];
        sprintf(param, "{\"port\":%d}", port);
        cli.Post("/node", param, "application/json");
    }

    cout<< "Listen : " << port << endl;
    svr.listen("0.0.0.0", port);

    return 0;
}

bool checkPort(int port){
    httplib::Client cli("localhost", port);

    if (auto res = cli.Get("/hi")) {
        if (res->status == 200) {
            cout << res->body << std::endl;
        }
    } else {
        auto err = res.error();
        cout << err <<endl;
        return false;
    }

    return true;
}

void addBlock(int _port, Block& block){
    httplib::Client cli("localhost", _port);

    auto res = cli.Post("/block", block.GetJSON().c_str(), "application/json");
}

void mine(int _port, int difficulty, int begin, int end){

    httplib::Client cli("localhost", _port);

    char param[200];

    sprintf(param, "{\"difficulty\":%d,\"begin\":%d,\"end\":%d}", difficulty, begin, end);

    auto res = cli.Post("/mine", param, "application/json");
}