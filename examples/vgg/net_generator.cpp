#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main ()
{
    string net;
    double num;
    double start;
    double interval;

    cout<<"net type: "<<endl;
    cin>>net;
    cout<<"number of trial: "<<endl;
    cin>>num;
    cout<<"start lambda: "<<endl;
    cin>>start;
    cout<<"lambda interval: "<<endl;
    cin>>interval;

    for(int i=0;i<num;i++){
        cout<<i<<endl;
        double lambda = start+(double)i*interval;
        string content = "";
        content+="lambda: ";
        content+=to_string(lambda);
        content+="\n";

        ifstream infile(net+".txt");

        string outfile_name = "net_"+to_string(i+1)+".prototxt";
        ofstream outfile(outfile_name);

        int j;

        for(j=0 ; !infile.eof() ; j++) // get content of infile
            content += infile.get();

        j--;
        content.erase(content.end()-1);     // erase last character

        cout << j << " characters read...\n";
        infile.close();

        outfile << content;                 // output
        outfile.close();
    }
    return 0;


}