#include<simplecpp>
#include<math.h>
#include<fstream>
#include<ctype.h>
#define num 5
int n=1000;

struct currency;

vector<currency*> curr;
float arr[num][num];

	float logconvertor( int i , int j, float arr[][num])
	{
		float k=log10(arr[i][j]);
		return k;
	}

struct currency
{
	string name;
	size_t index;
	float dist;
	vector<currency*> anc;
	vector<currency*> neighbour;

	currency()
	{
		index=0;
		dist=-INFINITY;
	}

	void search(string finalcurr,float arr[][num])
	{	
		size_t j;
		for(size_t i=0;i<curr.size();i++)
		{
			if(i==index)
				continue;

				for( j=0;j<anc.size();j++)
				{
					if(i==anc[j]->index)break;
				}
				
			if(j<anc.size())continue;
			neighbour.push_back(curr[i]);
		}

		float tempd=0;
		for(size_t i=0; i<neighbour.size();i++)
		{
			tempd= dist + logconvertor(index,neighbour[i]->index,arr);

			if (tempd < neighbour[i]->dist) return;
			neighbour[i]->dist= tempd;
			for(vector<currency*>::iterator vi=neighbour[i]->anc.end();vi!=neighbour[i]->anc.begin();vi--)
			{
				neighbour[i]->anc.pop_back();
			}

			neighbour[i]->anc=anc;
			neighbour[i]->anc.push_back(this);

			if(neighbour[i]->name==finalcurr) continue;
			neighbour[i]->search(finalcurr,arr);
		}
	}
};

struct transaction
{
  bool type;
  int amount;
  int balance;
  transaction(bool f,int a,int b)
  {
  	type=f;
	amount=a;
	balance=b;
  }
};

struct customer
{
  int id;
  int balance;
  vector <transaction> t;
  string password;
  customer(int amount,string pwd)
  {
  	balance=amount;
  	id = n;
  	password=pwd;
  }

  void withdraw(int amount)
  {
        if(balance>=amount)
        {
            balance = balance - amount;
            transaction temp(1,amount,balance);
            t.push_back(temp);
            cout<<"Transaction Complete. New Account Balance: "<<balance<<" INR"<<endl;
        }
        else cout<<" Insufficient Funds. "<<endl;
  }

  void deposit(int amount)
  {
		balance = balance + amount;
		if(balance>200000)
		{
			cout<<"ACCOUNT BALANCE LIMIT EXCEEDED. TRY OPENING A NEW ACCOUNT"<<endl;
			balance = balance - amount;
			return;
		}	
		transaction temp(0,amount,balance);
		t.push_back(temp);
		cout<<"Transaction Complete. New Account Balance: "<<balance<<" INR"<<endl;
  }

  void display()
  {
  		for(size_t i=0;i<t.size();i++)
		{
			if(t[i].type==0)
			{
			    cout<<"Transaction Type : Deposit."<<endl;
				cout<<"Amount Deposited "<<t[i].amount<<" INR"<<". New Balance :"<<t[i].balance<<" INR"<<endl;
			}
			else
			{
			    cout<<"Transaction Type : Withdrawal. "<<endl;
				cout<<"Amount Withdrawn "<<t[i].amount<<" INR"<<". Balance Remaining : "<<t[i].balance<<" INR"<<endl;
			}
		}
		cout<<"Final balance :"<< balance<<" INR"<<endl;
  }

  void passchange()
  {
  	string temp;
  	cout<<"Enter old password:";
  	cin>>temp;
  	if(password==temp)
  	{
  		cout<<"Enter new password:";
  		cin>>temp;
  		string check;
  		cout<<"Re-enter new password:";
  		cin>>check;
  		if(check==temp)
  		{
  			if(password==temp)
  				cout<<"Password is same as old password, Change discarded"<<endl;

  			else
            {
                password=temp;
                cout<<" Password Succesfully Changed."<<endl;
  			}
  		}
  		else
  		{
  			cout<<"Passwords don't match.try again"<<endl;
  			passchange();
  		}
        return;
  	}
  	else
   	{
   		cout<<"Wrong password.Try again"<<endl;
   		passchange();
  	}
  }
};

struct bank
{
  int count;
  vector <customer> list;
  bank()
  {
  	count=0;
  }

  void open(customer p)
  {
  	list.push_back(p);
 	count++;
  }

  void close(customer p)
  {
  	for(vector<customer>::iterator i=list.begin();i<list.end();i++)
  	{
  		if(p.id==i->id)
  		{
  			list.erase(i);
  			count--;
  		}
  	}	
  	//vector<customer>::iterator vi = list.begin() + p -1000;
	//list.erase(vi-1);
	//count--;
  }

  void update(customer p)
  {
  		for(size_t i=0;i<list.size();i++)
		{
	  		if(list[i].id==p.id)
			{
				list[i].password=p.password;
				list[i].balance=p.balance;
				for(vector<transaction>::iterator vi=list[i].t.end();vi!=list[i].t.begin();vi--)
				{
					list[i].t.pop_back();
				}
				for(size_t j=0;j<p.t.size();j++)
				{
					list[i].t.push_back(p.t[j]);
				}
  	 		}
  		}
  }
};

void choice2(bank &ccb, customer p);

void amount(bank &ccb,customer &p,float exchange_rate,string nam)
{
	cout<<"Enter the amount you want to change :";
	int s;
	string amt;
	char ch;
	cin>>amt;
	for(size_t i=0;i<amt.size();i++)
	{	
		ch=amt[i];
		if(isdigit(ch))
			continue;
		else
		{
			cout<<"WRONG INPUT. PLEASE TRY AGAIN"<<endl;
			return amount(ccb,p,exchange_rate,nam);
		}
	}
	s=stoi(amt);

	if(p.balance<s)
	{
		cout<<"You do not have sufficient balance, please try again"<<endl;
		return amount(ccb,p,exchange_rate,nam);
	}
	p.withdraw(s);
	cout<<"You have received :"<<(exchange_rate*s)<<' '<<nam<<endl;
}

void currency_conversion(bank &ccb,customer &p)
{
	string finalcurr;
	cout<<"Available Currencies : ";
	for(size_t i=0;i<curr.size();i++)
    {
        cout<<curr[i]->name<<' ';
    }
    cout<<endl;
	cout<<"Enter the Currency you want to change to ";
	cin>>finalcurr;
	size_t i=0;int j;
	for(i=0;i<curr.size();i++)
	{
		if(curr[i]->name==finalcurr)	break;
	}
	if(i==curr.size())
	{
		cout<<"Invalid Currency "<<"try again"<<endl;
		return currency_conversion(ccb,p);
	}

	curr[0]->dist=0;								//assuming first entry is INR.
	curr[0]->search(finalcurr,arr);

	for(size_t j=0;j<curr[i]->anc.size();j++)
	{
		cout<<curr[i]->anc[j]->name<<" to ";		//finds the final currency
	}
	cout<<curr[i]->name<<endl;
	cout<<"Current Currency Exchange Rate is - "<<"1 INR gives "<<pow(10,curr[i]->dist)<<" "<<curr[i]->name<<endl;
	for(i=0;i<curr.size();i++)
	{
		if(curr[i]->name==finalcurr)
		{
			j=curr[i]->index;
			break;
		}
	}
	cout<<"Actual Currency Exchange Rate is - "<<"1 INR gives "<<arr[0][j]<<" "<<finalcurr<<endl;	
	cout<<"Do you want to convert your money to "<<finalcurr<<" : y/n "<<endl;
	char temp;
	string temp1;
	cin>>temp1;
	temp=temp1[0];
	if(temp1.size()>1) 
	{
		cout<<"WRONG INPUT. PLEASE TRY AGAIN"<<endl;
		return currency_conversion(ccb,p);
	}
	
	if(temp=='y')	return amount(ccb,p,pow(10,curr[i]->dist),finalcurr);
	else if(temp=='n') return;
	else 
	{
		cout<<"WRONG INPUT. PLEASE TRY AGAIN"<<endl;
		return currency_conversion(ccb,p);
	}	
}

void acc(bank &ccb,customer p);
void choice(bank &ccb);

void signout(bank &ccb, customer p);

void choice2(bank &ccb, customer p)
{
    cout<<"1.      PRESS b FOR BALANCE INQUIRY"<<endl;
	cout<<"2.	PRESS w FOR WITHDRAWAL"<<endl;
	cout<<"3.	PRESS d TO DEPOSIT"<<endl;
	cout<<"4.	PRESS t TO VIEW ALL TRANSACTIONS"<<endl;
	cout<<"5.	PRESS p TO CHANGE PASSWORD"<<endl;
	cout<<"6.	PRESS s TO SIGNOUT"<<endl;
	cout<<"7.	PRESS x TO CLOSE ACCOUNT"<<endl;
	cout<<"8.	PRESS c FOR CURRENCY CONVERTOR"<<endl;
	char ch;string temp;
	cin>>temp;
	ch=temp[0];
	if(temp.size()>1) 
	{
		cout<<"WRONG INPUT. PLEASE TRY AGAIN"<<endl;
		return choice2(ccb,p);
	}
	
	if(ch=='b')
    {
        cout<<"Your Current Balance is : "<<p.balance<<" INR"<<endl;
        return choice2(ccb,p);
    }

	if(ch=='w')
	{
		int s;
		string amt;
		char ch;
		cout<<"ENTER THE AMOUNT YOU WANT TO WITHDRAW"<<endl;
		cin>>amt;
		for(size_t i=0;i<amt.size();i++)
		{	
			ch=amt[i];
			if(isdigit(ch))
				continue;
			else
			{
				cout<<"WRONG INPUT. PLEASE TRY AGAIN"<<endl;
				return choice2(ccb,p);
			}
		}
		s=stoi(amt);
		if(s>25000) 
		{
			cout<<"WITHDRAW LIMIT EXCEEDED. PLEASE TRY AGAIN"<<endl;
			return choice2(ccb,p);
		}				
		p.withdraw(s);
		return choice2(ccb,p);
	}

	else if(ch=='d')
	{
		int s;string amt;char ch;
		cout<<"ENTER THE AMOUNT YOU WANT TO DEPOSIT"<<endl;
		cin>>amt;
		for(size_t i=0;i<amt.size();i++)
		{
			ch=amt[i];
			if(isdigit(ch))
				continue;
			else
			{
				cout<<"WRONG INPUT. PLEASE TRY AGAIN"<<endl;
				return choice2(ccb,p);
			}
		}
		s=stoi(amt);			
		if(s>25000) 
		{
			cout<<"DEPOSIT LIMIT EXCEEDED. PLEASE TRY AGAIN"<<endl;
			return choice2(ccb,p);
		}
		p.deposit(s);
		return choice2(ccb,p);
	}

	else if(ch=='t')
	{
		p.display();
		return choice2(ccb,p);
	}

	else if(ch=='p')
	{
		p.passchange();
		return choice2(ccb,p);
	}

	else if(ch=='s') signout(ccb,p);
	else if(ch=='x') 
	{
		acc(ccb,p);
		return choice(ccb);
	}	
	else if(ch=='c')
	{
		currency_conversion(ccb,p);
		return choice2(ccb,p);
	}

	else
	{
		cout<<"INVALID CHOICE"<<endl;
		return choice2(ccb,p);
	}
}



void signout(bank &ccb, customer p)
{
	cout<<"ARE YOU SURE: y/n"<<endl;
	char temp;
	string temp1;
	cin>>temp1;
	temp=temp1[0];
	if(temp1.size()>1) 
	{
		cout<<"WRONG INPUT. PLEASE TRY AGAIN"<<endl;
		return choice(ccb);
	}
	if(temp=='y')
	{
		ccb.update(p);
		return choice(ccb);
	}
	else if(temp=='n') return choice2(ccb,p);
	else 
	{
		cout<<"WRONG INPUT. PLEASE TRY AGAIN"<<endl;
		return choice(ccb);
	}	
}

void acc(bank &ccb,customer p)
{
	cout<<"ARE YOU SURE YOU WANT TO CLOSE ACC : y/n"<<endl;
    char tmp;
	cin>>tmp;
	if(tmp=='y')
    {
        cout<<"Enter Password to Confirm Closing Account: ";
        string temp;
        cin>>temp;
        if(temp==p.password)
        {
            return ccb.close(p);
        }
        else
        {
            cout<<"Wrong Password."; acc(ccb,p);
        }
    }
	else if(tmp=='n') return choice2(ccb,p);
}

void login(bank &ccb)
{
	cout<<"ENTER YOUR USERID : ";
	int userid;
	string password,username;
	char ch;
	cin>>username;
	for(size_t i=0;i<username.size();i++)
	{
		ch=username[i];
		if(isdigit(ch))
			continue;
		else
		{
			cout<<"WRONG INPUT. PLEASE TRY AGAIN"<<endl;
			return login(ccb);
		}
	}
	userid=stoi(username);
	
	cout<<"ENTER PASSWORD : ";
	cin>>password;
	for(size_t i=0;i<ccb.list.size();i++)
	{
		if(userid==ccb.list[i].id&&password==ccb.list[i].password)
		{
			cout<<"Successfully logged in"<<endl;
 			return choice2(ccb,ccb.list[i]);
		}
	}
	cout<<"USERNAME OR PASSWORD DO NOT MATCH, TRY AGAIN"<<endl;
	return login(ccb);

}

void signup(bank &ccb)
{
	string password,amt;int bal;char ch;
	cout<<"ENTER PASSWORD: ";
	cin>>password;
	cout<<"ENTER OPENING BALANCE: ";
	cin>>amt;
	for(size_t i=0;i<amt.size();i++)
	{
		ch=amt[i];
		if(isdigit(ch))
			continue;
		else
		{
			cout<<"WRONG INPUT. PLEASE TRY AGAIN"<<endl;
			return signup(ccb);
		}
	}
	bal=stoi(amt);
	if(bal>200000) 
	{
		cout<<"ACCOUNT BALANCE LIMIT EXCEEDED."<<endl;
		return signup(ccb);
	}				
	
	customer p(bal,password);
	ccb.open(p);
	n++;
	cout<<"YOUR ACCOUNT HAS BEEN CREATED IN XXX BANK"<<endl;
	cout<<"YOUR USER ID is: "<<p.id<<endl;
	cout<<"YOU CAN LOGIN NOW"<<endl;
	login(ccb);
}

void choice(bank &ccb)
{
	cout<<"PRESS l FOR LOGIN OR s FOR SIGN UP OR e TO EXIT"<<endl;
	char ch;
	string temp;
	cin>>temp;
	ch=temp[0];
	if(temp.size()>1) 
	{
		cout<<"WRONG INPUT. PLEASE TRY AGAIN"<<endl;
		return choice(ccb);
	}
	
	if(ch=='l')
	{
		login(ccb);
		return;
	}
	else if(ch=='s')
	{
		signup(ccb);
		return;
	}
	else if(ch=='e')
	{
		cout<<"ARE YOU SURE: y/n"<<endl;
		char temp;
		string temp1;
		cin>>temp1;
		temp=temp1[0];
		if(temp1.size()>1) 
		{
			cout<<"WRONG INPUT. PLEASE TRY AGAIN"<<endl;
			return choice(ccb);
		}
		if(temp=='y')  return;
		else if(temp=='n') return choice(ccb);
		else 
		{
			cout<<"WRONG INPUT. PLEASE TRY AGAIN"<<endl;
			return choice(ccb);
		}	
	}
	else
	{
		cout<<"INVALID CHOICE"<<endl;
		choice(ccb);
	}
}

int main()
{
  cout<<".................WELCOME TO QUANTIZED INTERNATIONAL BANK................."<<endl;
  ifstream in("customerdetails.txt",ios::in);
  ifstream ain("transactions.txt",ios::in);
  int id1,id2,no;
  string pwd;
  bool typ;

  int bal,amt,blnce;
  bank ccb;
  while(in>>id1)
  {
  	ain>>id2;
  	if(id1==id2)
  	{
  	  ain>>no;
  	  in>>pwd;
  	  in>>bal;
  	  customer a(bal,pwd);
  	  repeat(no)
  	  {
  	    ain>>typ>>amt>>blnce;
  	    transaction c(typ,amt,blnce);
  	    a.t.push_back(c);
  	  }
  	  a.id=id1;
  	  ccb.open(a);
    }
  }
  n=id1 + 1;

  ifstream inc("CURRENCY_CONVERTER.txt",ios::in);
	size_t count=0;
	float c;

	string temp;
	repeat(num)
	{
		currency* cc=new currency;
		inc>>cc->name;
		cc->index=count;
		curr.push_back(cc);
		count++;
	}

	for(int i=0;i<num;i++)
	{
		for(int j=0;j<num;j++)
		{
			inc>>c;
			arr[i][j]=c;
		}
	}

  choice(ccb);

  ofstream dout("customerdetails.txt",ios::out);
  ofstream eout("transactions.txt",ios::out);
  for(size_t i=0;i<ccb.list.size();i++)
  {
  	dout<<ccb.list[i].id<<' '<<ccb.list[i].password<<' '<<ccb.list[i].balance<<endl;
	eout<<ccb.list[i].id<<' '<<ccb.list[i].t.size()<<endl;
	for(size_t j=0;j<ccb.list[i].t.size();j++)
		eout<<ccb.list[i].t[j].type<<' '<<ccb.list[i].t[j].amount<<' '<<ccb.list[i].t[j].balance<<endl;
  }
}
