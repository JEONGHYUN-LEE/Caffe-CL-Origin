#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#define GetCurrentDir getcwd

using namespace std;
string current_path( void ) {
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    int n = current_working_dir.find("caffe-original/");
    string ret = current_working_dir.substr(n+15,current_working_dir.size());
    return ret;
}

int main ()
{
    string current = current_path();
    string net_path="";
    string method;
    string snapshot="";
    int gpu_id;
    int num;


    cout<<"method name: "<<endl;
    cin>>method;
    cout<<"gpu_id: "<<endl;
    cin>>gpu_id;
    cout<<"number of trial: "<<endl;
    cin>>num;

    net_path +=current_path();
    snapshot +=current_path();
    snapshot +="/snapshot";
    for(int i=0;i<num;i++){
        string content = "";
        string net = net_path;
        net +=("/net_"+to_string(i+1)+".prototxt");
        content+="net: \""+net+"\"\n";
        content+="test_iter: 100\n";
        content+="test_interval: 1000\n";
        content+= "base_lr: 0.001\n";
        content+= "momentum: 0.9\n";
        content+="momentum2: 0.999\n";
        content+="lr_policy: \"fixed\"\n";
        content+="display: 100\n";
        content+="max_iter: 1000\n";
        content+="regularization_type: \"";
        content+=method;
        content+="\"\n";
        if(method == "elastic"){
            content+="weight_decay: 0.001\n";
        }
        content+="snapshot_prefix:";
        content+="\"";
        content+=snapshot;
        content+="/train";
        content+=to_string(i+1);
        content+="-\"\n";
        content+="type: \"Adam\"\n";
        content+="solver_mode: GPU\n";
        content+="device_id: ";
        content+=to_string(gpu_id);
        content+="\n";
        string outfile_name = "solver_"+to_string(i+1)+".prototxt";
        ofstream outfile(outfile_name);
        outfile << content;
        outfile.close();
    }

    for(int i=0;i<num;i++){
        string content = "";
        string net = net_path;
        net +=("/net_"+to_string(i+1)+".prototxt");
        content+="net: \""+net+"\"\n";
        content+="test_iter: 100\n";
        content+="test_interval: 1000\n";
        content+= "base_lr: 0.001\n";
        content+= "momentum: 0.9\n";
        content+="momentum2: 0.999\n";
        content+="lr_policy: \"fixed\"\n";
        content+="display: 100\n";
        content+="max_iter: 1000\n";
        content+="regularization_type: \"";
        content+=method;
        content+="\"\n";
        if(method == "elastic"){
            content+="weight_decay: 0\n";
        }
        content+="snapshot_prefix:";
        content+="\"";
        content+=snapshot;
        content+="/ret_train";
        content+=to_string(i+1);
        content+="-\"\n";
        content+="type: \"Adam\"\n";
        content+="solver_mode: GPU\n";
        content+="device_id: ";
        content+=to_string(gpu_id);
        content+="\n";
        string outfile_name = "ret_solver_"+to_string(i+1)+".prototxt";
        ofstream outfile(outfile_name);
        outfile << content;
        outfile.close();
    }
    ofstream outfile("train.sh");
    string content = "";
    for(int i=0;i<num;i++){
        content+="./build/tools/caffe train --solver=";
        content+=current;
        content+="/solver_";
        content+=to_string(i+1);
        content+=".prototxt\n";
    }
    for(int i=0;i<num;i++){
        content+="./build/tools/caffe train --solver=";
        content+=current;
        content+="/ret_solver_";
        content+=to_string(i+1);
        content+=".prototxt ";
        content+="-weights ";
        content+=current;
        content+="/snapshot/train";
        content+=to_string(i+1);
        content+="-_iter_1000.caffemodel\n";
    }
    outfile << content;
    outfile.close();
    return 0;


}
