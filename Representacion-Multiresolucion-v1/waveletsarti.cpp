#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
//#include <stdio.h>

//usar -U__STRICT_ANSI__ // tener en cuenta que sale error al querer cerrar en la ejecucion
	
	
using namespace std;
typedef double(*fun)(double);


class GNUplot
{
protected:
	FILE *gnuplotpipe;
public:
	GNUplot(){
		gnuplotpipe=popen("gnuplot -persist","w");
		if(!gnuplotpipe)	
			cerr<<("gnuplot no encontrado");
	}
	void operator()(const string &comand)
	{
		fprintf(gnuplotpipe,"%s\n",comand.c_str());
		fflush(gnuplotpipe);
	}
	~GNUplot()
	{
		fprintf(gnuplotpipe,"exit\n");
		pclose(gnuplotpipe);
	}
	
};




void mostrarv(vector<double> s){
	int u=s.size();
	for (int i=0;i<u;i++){
	cout<<s[i]<<" ";
	}
}
	
void mostrarm(int fila,int columna,vector<vector<double>> &A){
	for(int i=0;i<fila;i++){
		for (int j=0;j<columna;j++){
			cout<<A[i][j]<<" ";
		}
		cout<<endl;
	}
}
	
	
double fun1(double x){
return exp(-(pow(x,2)));}
		
double fun2(double x){
return x*x;}
			
			
double cuagauss(double a,double b,vector<vector<double>> matrix,fun f1){
	//para este caso tenemos que el intervalo es [0,1]
	double n=5;
	int fila=15;
	int colum=3;
	//es por ello que (b-a) es 1
	double t;
	double r=(b+a)/2;
	double s=(b-a)/2;
	for (double i=0;i<fila;i++){
		if ((matrix[i][0]>=n) && (matrix[i][0]<n+1)){
			double x=s*matrix[i][2]+r;
			t+=(matrix[i][1]*f1(x));
			}
		}
	t=s*t;
	return t;
}		
	
vector<double> nnodos(int n){//solo dar el  nivel en el que se
	double s=2;
	double r=1/2.;
	vector<double> lleno;
	if (n>=0){
		s=pow(s,n+1)+1;
		for (int i=0;i<s;i++)
			lleno.push_back(i*pow(r,n+1));
	}
	else {
		s=pow(s,abs(n)+1);
		for (int i=0;i<s;i++);
		lleno.push_back(0);}
	return lleno;
}
	
	
///////
vector<vector<double>> posi(int n){
	vector<double> cal=nnodos(n);
	vector<vector<double>> matrizde;
	for (int i=0;i<cal.size()-1;i++){
		
		vector<double> rel;
		for (int j=0; j<3;j++){
			if (j==0)
				rel.push_back(i);
			else if(j==1)
				rel.push_back(cal[i]);
			else if(j==2)
				rel.push_back(cal[i+1]);
		}
	matrizde.push_back(rel);
	}
 return matrizde;
} 
	
///////////////////////////////////////////
// Recien es el algoritmo de wavetes del articulo
// Damos el nivel que queremos K=1,...,J_(k-1) para ejemplo tomamos nivel 1


//int algowave(int w){	//prueba para datos en enteros

vector<double> algowaveida(int w,vector<vector<double>> matrix){ //prueva para datos en el vector
	
// w es el nivel que queremos	
//vector<vector<double>> algowave(int w){ 
	vector<vector<double>> r=posi(w);
	vector<vector<double>> s=posi(w-1);
	int Jnivels=pow(2,w)*2;//dimension del nivel que queremos ejem J_1=pow(2,1)*2 tomar en cuenta que J_0=2
	vector<double> b;
for (int i=0;i<Jnivels;i++){
	//vector <double> b(Jnivels); si quieres que salga el numerofijo de datos

	if (i<Jnivels/2){
		double v=(1/(s[i][2]-s[i][1]))*(((r[2*i][2]-r[2*i][1])*pow(2,w+1)*cuagauss(r[2*i][1],r[2*i][2],matrix,fun2))+((r[2*i+1][2]-r[2*i+1][1])*pow(2,w+1)*cuagauss(r[2*i+1][1],r[2*i+1][2],matrix,fun2)));
		//double v=pow(2,w+1)*cuagauss(r[2*i+1][1],r[2*i+1][2],matrix,fun2);
		//double v=pow(2,w+1)*cuagauss(r[2*i+1][1],r[2*i+1][2],matrix,fun2);
		
	    b.push_back(v);
	}
	else{
		int t=i-Jnivels/2;
		double d=2*((r[2*t][2]-r[2*t][1])/(s[t][2]-s[t][1]))*(pow(2,w+1)*cuagauss(r[2*t][1],r[2*t][2],matrix,fun2)-pow(2,w)*cuagauss(s[t][1],s[t][2],matrix,fun2));
		//double d=pow(2,w+1)*cuagauss(r[t][1],r[t+1][2],matrix,fun2);//el error 
		//double d=pow(2,w)*cuagauss(s[t],s[t+1],matrix,fun2);
		//double d=cuagauss(s[t][1],s[t][2],matrix,fun2);
		b.push_back(d);}
	}
return b;
}
	
	
		
//////////////////////////////////////////////////////////////////////////////

vector<double> invermulti(vector<double> nivelmr, vector<vector<double>> matrix){
	int w=log2(nivelmr.size()/2);
	int Jnivelsant=nivelmr.size();
	vector<vector<double>> r=posi(w);
	vector<vector<double>> s=posi(w-1);
	//dimension del nivel que queremos ejem J_1=pow(2,1)*2 tomar en cuenta que J_0=2
	vector<double> b;
	for (int i=0;i<Jnivelsant;i++){
		//vector <double> b(Jnivels); si quieres que salga el numerofijo de datos
		
		if ((i%2)==0){
			double va=(pow(2,w)*cuagauss(s[i/2][1],s[i/2][2],matrix,fun2))+((s[i/2][2]-s[i/2][1])/(2*(r[i+1][2]-r[i+1][1])))*nivelmr[(Jnivelsant+i)/2];
			//double va=((s[i/2][2]-s[i/2][1])/(2*(r[i+1][2]-r[i+1][1])));
			
			b.push_back(va);
		}
		else{
			int t=(i-1)/2;
			double vb=(1/(r[i][2]-r[i][1]))*((s[t][2]-s[t][1])*(pow(2,w)*cuagauss(s[t][1],s[t][2],matrix,fun2))-((r[2*t][2]-r[2*t][1])*pow(2,w+1)*cuagauss(r[2*t][1],r[2*t][2],matrix,fun2)));
			//double vb=((r[2*t][2]-r[2*t][1])*pow(2,w+1)*cuagauss(r[2*t][1],r[2*t][2],matrix,fun2));//el error 
			//double d=pow(2,w)*cuagauss(s[t],s[t+1],matrix,fun2);
			//double d=cuagauss(s[t][1],s[t][2],matrix,fun2);
			b.push_back(vb);}
	}
	return b;
}
	
	

	
	
/////////////////////////////////////////////////////////////////////////////	

int main() {
	
//////////////////////////////////// Tabla de Cuadratura gausseana
	
ifstream input;
input.open("tablagauss.txt");
vector<vector<double>> matrix;
int fila=15;
int colum=3;
while(!input.eof())
{
	for (int i=0;i<fila;i++){
		vector<double> temp;
		double value;
		for (int j=0;j<colum;j++){
			input>>value;
			temp.push_back(value);
		}
		matrix.push_back(temp);
	}
	input.close();
}


///////////////////////////////////////////////////////////


//mostrarm(15,3,matrix);
//cout<<endl;
//double integralyo=cuagauss(1,1.5,matrix,fun2);
//cout<<integralyo<<endl;

vector<double> car=algowaveida(12,matrix);
//mostrarv(car);
//cout<<endl;
////vector<vector <double>> galon=posi(0);
////mostrarm(2,3,galon);

vector<double> tur=invermulti(car,matrix);
//mostrarv(tur);
//cout<<endl;

//int n=log2(4/2);
//cout<<n<<endl;


int fur=tur.size();//log2(tur.size()/2);
//cout<<fur<<endl;
int niveld=log2(tur.size()/2);
//cout<<niveld<<endl;
vector<vector<double>> fiar=posi(niveld);
//mostrarm(4,3,fiar);
//cout<<endl;

ofstream og("wavelet.txt");
for (int i=0;i<fur-1;i++){
og<<fiar[i][1]<<" "<<tur[i]<<endl;
og<<fiar[i+1][1]<<" "<<tur[i]<<endl;
}
og<<fiar[fur-1][1]<<" "<<tur[fur-1]<<endl;
og<<fiar[fur-1][2]<<" "<<tur[fur-1]<<endl;

GNUplot P;
P("plot \'wavelet.txt \'with lines");

	
	
	
	
	

	cout<<endl;	
	return 0;
}

