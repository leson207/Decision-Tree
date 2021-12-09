#include <bits/stdc++.h>
using namespace std;
using namespace std;
const int number_of_atk=5;
struct data
{
    char label;
    int atk[number_of_atk+1];
    data(char l,int a,int b,int c,int d)
    {
        label=l;
        atk[1]=a;
        atk[2]=b;
        atk[3]=c;
        atk[4]=d;
        atk[5]=(a+b)-(c+d);
        //atk[5]=((a+b)>(c+d));
        //atk[6]=((a*b)-(c*d)==0);
        //if(a*b>c*d) atk[5]=-1;
        //if(a*b==c*d) atk[5]=0;
        //if(a*b<c*d) atk[5]=1;
    }
    void out()
    {
        cout<<label;
        for(int i=1;i<=number_of_atk;i++) cout<<" "<<atk[i];
        cout<<endl;
        return;
    }
};
struct node
{
    int choosenatk;
    bool check_box[number_of_atk+1];
    vector<int> choosenvalue;
    char leaf_value;
    vector<data> current_data;
    vector<node*> nextnode;
    bool is_choosen_value(int val)
    {
        int i;
        for(i=0;i<choosenvalue.size();i++) if(choosenvalue[i]==val) return true;
        return false;
    }
};
vector<data> query,dataset[3];
void delete_tree(node *root)
{
    for(int i=0;i<root->nextnode.size();i++) delete_tree(root->nextnode[i]);
    root->nextnode.clear();
    delete root;
    return;
}
int convert(char c)
{
    if(c=='L') return 0;
    if(c=='B') return 1;
    if(c=='R') return 2;
}
long long Rand(long long l, long long h)
{
    return l + ((long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (long long)rand() * (RAND_MAX + 1) +
                rand()) % (h - l + 1);
}
void label_cnt(int &l,int &b,int &r,vector<data> &v)
{
    int i;
    for(i=0;i<v.size();i++)
    {
        if(v[i].label=='L') l++;
        if(v[i].label=='B') b++;
        if(v[i].label=='R') r++;
    }
    return;
}
double entropy(vector<data> &v)
{
    int i;
    int cnt[3];
    cnt[0]=cnt[1]=cnt[2]=0;
    double sum=(double)v.size(),res=0;
    if(sum==0) return 1;
    label_cnt(cnt[0],cnt[1],cnt[2],v);
    for(i=0;i<3;i++)
    {
        if(cnt[i]==0) continue;
        res=res-(cnt[i]/sum)*log(cnt[i]/sum);
    }
    return res;
}
double gainratio(vector<pair<char,pair<int,double> > > &entropy_list,vector<int> &size_list)
{
    int i;
    double sum=0,res=0,splitInfo=0;
    for(i=0;i<size_list.size();i++) sum=sum+size_list[0];
    if(sum==0) return 1;
    for(i=0;i<size_list.size();i++)
        splitInfo=splitInfo-(size_list[i]/sum)*log(size_list[i]/sum);
    for(i=0;i<entropy_list.size();i++)
        res=res+size_list[i]*entropy_list[i].second.second;
    res=res/sum;
    return res/splitInfo;
}
bool tmp(pair<int,double> a,pair<int,double> b)
{
    return a.second<b.second || (a.second==b.second && b.first==1);
}
char getmax_label(int a,int b,int c)
{
    vector<pair<int,double> > temp;
    temp.push_back({0,a});
    temp.push_back({1,b});
    temp.push_back({2,c});
    sort(temp.begin(),temp.end(),tmp);
    if(temp[2].first==0) return 'L';
    if(temp[2].first==1) return 'B';
    if(temp[2].first==2) return 'R';
}
void pich_leaf_value(node *root)
{
    int i,j;
    int cnt[3];
    cnt[0]=cnt[1]=cnt[2]=0;
    label_cnt(cnt[0],cnt[1],cnt[2],root->current_data);
    root->leaf_value=getmax_label(cnt[0],cnt[1],cnt[2]);
    return;
}
bool tmp1(pair<char,pair<int,double> > a,pair<char,pair<int,double> >  b)
{
    return a.second.second<b.second.second || (a.second.second==b.second.second && b.second.first==1);
}
void xuli_atk(int now_atk,vector<data> &v,vector<pair<char,pair<int,double> > > &now_entropy_list,vector<pair<int,double> > &infogainlist)
{
    int i,j;
    int cnt[3];
    cnt[0]=cnt[1]=cnt[2]=0;
    vector<int> size_list;
    vector<vector<data> > classify;
    label_cnt(cnt[0],cnt[1],cnt[2],v);
    for(i=0;i<v.size();i++)
    {
        for(j=0;j<classify.size();j++)
        {
            if(classify[j][0].atk[now_atk]==v[i].atk[now_atk])
            {
                classify[j].push_back(v[i]);
                break;
            }
        }
        if(j==classify.size())
        {
            vector<data> temp;
            temp.push_back(v[i]);
            classify.push_back(temp);
        }
    }
    for(i=0;i<classify.size();i++)
    {
        size_list.push_back(classify[i].size());
        now_entropy_list.push_back({classify[i][0].label,{classify[i][0].atk[now_atk],entropy(classify[i])}});
    }
    infogainlist.push_back({now_atk,gainratio(now_entropy_list,size_list)});
    sort(now_entropy_list.begin(),now_entropy_list.end(),tmp1);
    return;
}
void Build_Node(node *root,queue<node*> &q)
{
    int i,j,k,t,new_node_cnt=-1,choosen_atk;
    vector<pair<char,pair<int,double> > > entropy_list[number_of_atk+1];
    vector<pair<int,double> > info_gain_list;

    for(i=1;i<=number_of_atk;i++)
    {
        if(root->check_box[i]==true) continue;
        xuli_atk(i,root->current_data,entropy_list[i],info_gain_list);
    }
    sort(info_gain_list.begin(),info_gain_list.end(),tmp);

    if(info_gain_list.size()==0 || info_gain_list[0].second==1.0)
    {
        pich_leaf_value(root);
        return;
    }
    choosen_atk=info_gain_list[0].first;
    //tao node
    for(i=0;i<entropy_list[choosen_atk].size();i++)
    {
        if(new_node_cnt==-1 || entropy_list[choosen_atk][i].second.second!=entropy_list[choosen_atk][i-1].second.second || entropy_list[choosen_atk][i].second.second==0)
        {
            new_node_cnt++;
            root->nextnode.push_back(new node());
            root->nextnode[new_node_cnt]->choosenatk=choosen_atk;
            root->nextnode[new_node_cnt]->check_box[choosen_atk]=true;
            if(entropy_list[choosen_atk][i].second.second==0)
                root->nextnode[new_node_cnt]->leaf_value=entropy_list[choosen_atk][i].first;
        }
        root->nextnode[new_node_cnt]->choosenvalue.push_back(entropy_list[choosen_atk][i].second.first);
    }
    //cap nhat node datalist
    for(t=0;t<=new_node_cnt;t++)
    {
        for(i=1;i<=number_of_atk;i++)
            if(root->check_box[i]==true) root->nextnode[t]->check_box[i]=root->check_box[i];
        for(i=0;i<root->current_data.size();i++)
        {
            data x=root->current_data[i];
            if(root->nextnode[t]->is_choosen_value(x.atk[choosen_atk])==1) root->nextnode[t]->current_data.push_back(x);
            //if(root->nextnode[t]->is_choosen_value(root->current_data[i].atk[choosen_atk])==1) root->nextnode[t]->current_data.push_back(root->current_data[i]);
        }
    }
    if(new_node_cnt==-1)
    {
        pich_leaf_value(root);
        return;
    }
    for(i=0;i<=new_node_cnt;i++)
    {
        if(root->nextnode[i]->leaf_value==NULL)
        q.push(root->nextnode[i]);
    }
    return;
}
void ID3Build(node *root,int min_size,int max_node)
{
    int cnt;
    queue<node*> q;
    q.push(root);
    while(!q.empty())
    {
        if(q.front()->current_data.size()<min_size || cnt>=max_node)
        {
            pich_leaf_value(q.front());
            q.pop();
            continue;
        }
        cnt++;
        Build_Node(q.front(),q);
        q.pop();
    }
    return;
}
char random_label()
{
    int x=Rand(0,2);
    if(x==0) return 'L';
    if(x==1) return 'B';
    if(x==2) return 'R';
}
char get_label(data &x,node *root)
{
    if(root->leaf_value!=NULL) return root->leaf_value;
    int i,atk_value;
    for(i=0;i<root->nextnode.size();i++)
    {
        atk_value=x.atk[root->nextnode[i]->choosenatk];
        if(root->nextnode[i]->is_choosen_value(atk_value)==true) return get_label(x,root->nextnode[i]);
    }
    return random_label();
    return '0';
}
void printf_tree(node *root,int lv)
{
    int i,j;
    if(lv==0) cout<<"root"<<endl;lv++;
    for(i=0;i<root->nextnode.size();i++)
    {
        for(j=1;j<=lv;j++) cout<<"   ";
        cout<<"atk "<<root->nextnode[i]->choosenatk<<": ";
        for(j=0;j<root->nextnode[i]->choosenvalue.size();j++) cout<<" "<<root->nextnode[i]->choosenvalue[j];
        if(root->nextnode[i]->leaf_value!=NULL) cout<<" "<<root->nextnode[i]->leaf_value;
        cout<<endl;
        printf_tree(root->nextnode[i],lv+1);
    }
    return;
}
void read_data(string path)
{
    string s;
    ifstream inFile;
    inFile.open(path);
    while(inFile>>s)
    {
        data x=data(s[0],s[2]-'0',s[4]-'0',s[6]-'0',s[8]-'0');
        if(s[0]=='L') dataset[0].push_back(x);
        if(s[0]=='B') dataset[1].push_back(x);
        if(s[0]=='R') dataset[2].push_back(x);
    }
    inFile.close();
    return;
}
double f1Score(vector<char> &a,vector<data> &b)
{
    int i,j;
    double precision,recall;
    double f1[3],cnt[3][3];
    for(i=0;i<3;i++)
    {
        f1[i]=0;
        for(j=0;j<3;j++) cnt[i][j]=0;
    }
    for(i=0;i<a.size();i++) cnt[convert(a[i])][convert(b[i].label)]+=1.0;
//    for(i=0;i<3;i++)
//    {
//        for(j=0;j<3;j++) cout<<cnt[i][j]<<" ";
//        cout<<endl;
//    }
//    cout<<endl;
    for(i=0;i<3;i++)
    {
        if(cnt[i][i]==0 && cnt[i][i]+cnt[(i+1)%3][i]+cnt[(i+2)%3][i]==0) {f1[i]=1;continue;}
        if(cnt[i][i]==0 && cnt[i][i]+cnt[(i+1)%3][i]+cnt[(i+2)%3][i]!=0) {f1[i]=0;continue;}
        precision=cnt[i][i]/(cnt[i][i]+cnt[(i+1)%3][i]+cnt[(i+2)%3][i]);
        recall=cnt[i][i]/(cnt[i][i]+cnt[i][(i+1)%3]+cnt[i][(i+2)%3]);

        f1[i]=1/precision+1/recall;
        f1[i]=2/f1[i];
    }
    return f1[0]*f1[1]*f1[2];
}
void random_forest()
{
    char c;
    double trust;
    double rate=1/4.0;
    vector<char> ans;
    int i,j,pos,tree,data_size[3];
    data_size[0]=dataset[0].size();
    data_size[1]=dataset[1].size();
    data_size[2]=dataset[2].size();
    double info[query.size()][3];//truy van-tong gia tri tin tuong l-b-r
    for(tree=1;tree<=200;tree++)
    {
        vector<data> query_test;
        for(i=0;i<=2;i++)
        {
            for(j=1;j<=data_size[i]*rate;j++)
            {
                pos=Rand(0,data_size[i]-j);
                query_test.push_back(dataset[i][pos]);
                dataset[i].erase(dataset[i].begin()+pos);
            }
            data_size[i]-=data_size[i]*rate;
        }


        node *root=new node();
        for(i=0;i<=2;i++)
        {
            for(j=1;j<=data_size[i]*(1-rate);j++)
            {
                pos=Rand(0,data_size[i]-1);
                root->current_data.push_back(dataset[i][pos]);
            }
        }

        for(i=1;i<=Rand(0,number_of_atk-1);i++)
            root->check_box[Rand(0,number_of_atk)]=1;

        //65-88
        //93-88
        //33-57
        //36-62
        ID3Build(root,68,50);
        //ID3Build(root,20,100);

        for(i=0;i<query_test.size();i++)
            ans.push_back(get_label(query_test[i],root));
        trust=f1Score(ans,query_test);
        for(i=0;i<query.size();i++)
        {
            c=get_label(query[i],root);
            info[i][convert(c)]+=trust;
        }
        for(i=0;i<query_test.size();i++)
        {
            data_size[convert(query_test[i].label)]++;
            dataset[convert(query_test[i].label)].push_back(query_test[i]);
        }
        delete_tree(root);
        ans.clear();
    }


//    rate=2/9.0;
//    for(tree=1;tree<=200;tree++)
//    {
//        vector<data> query_test;
//        for(i=0;i<=2;i++)
//        {
//            for(j=1;j<=data_size[1]*rate;j++)
//            {
//                pos=Rand(0,data_size[i]-j);
//                query_test.push_back(dataset[i][pos]);
//                dataset[i].erase(dataset[i].begin()+pos);
//            }
//            data_size[i]-=data_size[i]*rate;
//        }
//        node *root=new node();
//        for(i=0;i<=2;i++)
//        {
//            for(j=1;j<=data_size[1]*(1-rate);j++)
//            {
//                pos=Rand(0,data_size[i]-1);
//                root->current_data.push_back(dataset[i][pos]);
//            }
//        }
//        for(i=1;i<=Rand(0,number_of_atk-1);i++)
//            root->check_box[Rand(0,number_of_atk)]=1;
//        //65-88
//        //93-88
//        //33-57
//        //36-62
//        ID3Build(root,2,24);
//
//        for(i=0;i<query_test.size();i++)
//            ans.push_back(get_label(query_test[i],root));
//        trust=f1Score(ans,query_test);
//        for(i=0;i<query.size();i++)
//        {
//            c=get_label(query[i],root);
//            info[i][convert(c)]+=trust;
//        }
//        for(i=0;i<query_test.size();i++)
//        {
//            data_size[convert(query_test[i].label)]++;
//            dataset[convert(query_test[i].label)].push_back(query_test[i]);
//        }
//        delete_tree(root);
//        ans.clear();
//    }
    trust=0;
    for(i=0;i<query.size();i++)
    {
        c=getmax_label(info[i][0],info[i][1],info[i][2]);
        ans.push_back(c);
        if(c==query[i].label) trust++;
    }
    cout<<f1Score(ans,query)<<endl;
    cout<<trust<<endl;
    return;
}
int normal_calc(int x,int y)
{
    int i,cnt=0;
    node *root=new node();
    for(i=0;i<dataset[0].size();i++) root->current_data.push_back(dataset[0][i]);
    for(i=0;i<dataset[1].size();i++) root->current_data.push_back(dataset[1][i]);
    for(i=0;i<dataset[2].size();i++) root->current_data.push_back(dataset[2][i]);
    ID3Build(root,x,y);
    vector<char> ans;
    for(i=0;i<query.size();i++)
    {
        ans.push_back(get_label(query[i],root));
        cout<<ans[i]<<endl;
        if(get_label(query[i],root)==query[i].label) cnt++;
    }
    //printf_tree(root,0);
    delete_tree(root);
    cout<<f1Score(ans,query)<<endl;
    cout<<cnt<<" "<<query.size()<<endl;
    return cnt;
}
bool tmp5(pair<int,pair<int,int> > a,pair<int,pair<int,int> > b)
{
    return a.first>b.first;
}
void get_max_pos()
{
    int i,j;
    vector<pair<int,pair<int,int> > > v;
    for(i=2;i<=100;i++)
        for(j=20;j<=150;j++) v.push_back({normal_calc(i,j),{i,j}});
    sort(v.begin(),v.end(),tmp5);
    for(i=0;i<=200;i++) cout<<v[i].first<<" "<<v[i].second.first<<" "<<v[i].second.second<<endl;
    return;
}
int main()
{
    #define file ""
    //freopen(file".txt","r",stdin);
    //freopen(file".out","w",stdout);
    //freopen("ans.out","w",stdout);
    //ios_base::sync_with_stdio(0);
    //cin.tie(0);cout.tie(0);
    string s;
    ifstream inFile;
    read_data("train.txt");
    //read_data("valid.txt");

    //inFile.open("private.txt");
    inFile.open("valid.txt");
    while(inFile>>s)
    {
        //data x=data('0',s[0]-'0',s[2]-'0',s[4]-'0',s[6]-'0');
        data x=data(s[0],s[2]-'0',s[4]-'0',s[6]-'0',s[8]-'0');
        query.push_back(x);
    }
    inFile.close();
    random_forest();
    //get_max_pos();
    //normal_calc(68,36);
    return 0;
}
