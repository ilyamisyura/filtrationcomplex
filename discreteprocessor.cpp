#include "discreteprocessor.h"

DiscreteProcessor::DiscreteProcessor(QObject *parent) :
    QObject(parent)
{
}

//QVector <double> DiscreteProcessor::getLambdaVector(int num, double noiseSigma, double signalSigma){
//    QVector <double> result;
//    for (int i=0; i<num; i++)
//    {
//        result.insert(i,1+2 * pow(noiseSigma/signalSigma,2) * (1-cos((i+1)*PI/(num+1))));
//    }
//    return result;
//}

//double DiscreteProcessor::Hm1(int i, int j, QVector <double> mu, double ** F) {
//    double sum3;
//    sum3=0;
//        for (int k=0; k<=mu.size()-1; k++)
//        {
//            if((i!=-1)&&(j!=-1)){
//                sum3+=1/mu.value(k)*F[i][k]*F[j][k];
//            }
//        }
//    return sum3;
//}

//QVector <double> DiscreteProcessor::Cm1Y(int n, double sigs, double alpha, QVector <double> Y, QVector <double> vecres)//C^(-1)*Y
//{
//    QVector <double> res;
//    vecres.clear();

//    vecres.insert(0,(1/(sigs*sigs))*(2*Y.value(0)-Y.value(1)));
//    for (int j=1; j<=n-2; j++)
//    {
//        vecres.insert(j,(1/(sigs*sigs))*(-Y.value(j-1)+2*Y.value(j)-Y.value(j+1)));
//    }
//    vecres.insert(n-1,(1/(sigs*sigs))*(-Y.value(n-2)+Y.value(n-1)));

//    return res;
//}

//QVector <double> DiscreteProcessor::simpleFilter(QVector <double> signal, double signalSigma, double noiseSigma){

//    QVector <double> filteredSignal;
//    QVector <double> result;
//    QVector <double> lambdaVector;
//    QVector <double> uu;

//    double SVlength;

//    int num;
//    num = signal.size();
//    filteredSignal.fill(0,num);


//    lambdaVector = getLambdaVector(num,noiseSigma,signalSigma);
////    for (int i=0; i<num; i++)
////    {
////        lambdaVector.insert(i,1+2 * pow(noiseSigma/signalSigma,2) * (1-cos((i+1)*PI/(num+1))));
////    }

//    double ** F;
//    F=(double **) malloc(num*(sizeof (double*)));
//    for (int i=0;i<num;i++)
//       F[i]=(double*) malloc(num*(sizeof (double)));

//    double sum;
//    double mm;

//    for (int i=0; i<num; i++)
//    {
//        for (int j=0; j<num; j++){
//            F[i][j]=sin((i+1)*(j+1)*PI/(num+1));
//        }
//    }

//    SVlength = 0;
//    for (int i=0; i<num; i++){
//        SVlength+=F[i][0]*F[i][0];
//    }

//    SVlength=sqrt(SVlength);

//    for (int i=0; i<num; i++)
//    {
//        for (int j=0; j<num; j++){
//            F[i][j]=F[i][j]/SVlength;
//        }
//    }

//    for (int kk=0; kk<num; kk++)
//    {
//        sum=0;
//        uu.clear();
//        for (int i=0; i<num; i++){
//            sum+= F[i][kk]*signal.value(i);
//        }

//        for (int i=0; i<num; i++){
//            uu.insert(i,F[i][kk]*sum/lambdaVector.value(kk));
//        }

//        mm=0;
//        for (int i=0; i<num; i++)
//        {
//            mm = filteredSignal.value(i);
//            filteredSignal.replace(i,mm+uu.value(i));
//        }
//    }
//    return filteredSignal;
//}

//DataSignal* DiscreteProcessor::bayesFilter(DataSignal signal, double signalSigma, double noiseSigma, double alpha){

//    int mini, minj;
//    int Limin, Lij;
//    int JumpJ;
//    int num;

//    double SVlength;

//    QVector <double> Yi;
//    QVector <double> LamVec;
//    QVector <double> C1Y;
//    QVector <double> H1bi;
//    QVector <double> MyX;
//    QVector <double> MyXF; //simply filtered signal

//    double ** F;
//    F=(double **) malloc(num*(sizeof (double*)));
//    for (int i=0;i<num;i++)
//       F[i]=(double*) malloc(num*(sizeof (double)));

//    for (int i=0; i<num; i++)
//    {
//        for (int j=0; j<num; j++){
//            F[i][j]=sin((i+1)*(j+1)*PI/(num+1));
//        }
//    }

//    SVlength = 0;
//    for (int i=0; i<num; i++){
//        SVlength+=F[i][0]*F[i][0];
//    }

//    SVlength=sqrt(SVlength);

//    for (int i=0; i<num; i++)
//    {
//        for (int j=0; j<num; j++){
//            F[i][j]=F[i][j]/SVlength;
//        }
//    }


//    minj=999999;
//    Lij=999999;


//    num = signal.size();

//    LamVec = getLambdaVector(num,noiseSigma,signalSigma);

//    for (int g=0; g<=num-3-1; g=g+3)
//    {
//        mini=g;
//        Limin=999999;
//        int ii=g;
//        int c=g;
//        do
//        {

//            double Li=0;
//            double XmYi=0;// (X-Yi,X-Yi)
//            double lastsk=0;
//            Yi.clear();
//            Yi.fill(0,num);
//            C1Y.clear();
//            C1Y.fill(0,num);
//            H1bi.clear();
//            H1bi.fill(0,num);
//            double biy=0;

//            //Сделать условие!!!!!!!!

//            biy+=(sqrt(1-1/pow(alpha,2))*(MyXF.value(ii-1)-MyXF.value(ii)))/
//                (1-((pow(signalSigma,2)/pow(noiseSigma,2))*(1-1/pow(alpha,2))*
//                (Hm1(ii-1,ii-1,LamVec,F)-Hm1(ii-1,ii,LamVec,F)-
//                Hm1(ii,ii-1,LamVec,F)+Hm1(ii,ii,LamVec,F))));//counting mu
//            double tmpH=H1bi.value(ii);
//            H1bi[ii]=tmpH+sqrt(1-1/(alpha*alpha))*(Hm1(ii,ii-1,LamVec,F)-Hm1(ii,JumpJ,LamVec,F));//H-1bi произведение

//            for (int i=0; i<num; i++)
//            {
//                double mm1=Yi.value(i);
//                Yi.insert(i,mm1+(MyXF.value(i)+(noiseSigma*noiseSigma/(signalSigma*signalSigma)*biy*H1bi.value(i))));// вычисление вектора Yi
//            }

//            Cm1Y(num,signalSigma,alpha,Yi,C1Y);//Получаем произведение обратной ковариационной матрицы и Yi
//            for (int i=0; i<=num-1; i++)
//            {
//                XmYi+=(MyX.value(i)-Yi.value(i))*(MyX.value(i)-Yi.value(i)); //Скалярное произведение (X-Yi,X-Yi)
//                lastsk+=C1Y.value(i)*MyY.value(i);
//            }

//            Li=XmYi/(noiseSigma*noiseSigma)+lastsk;
//            if(Li<Limin)
//            {
//            mini=ii;
//            Limin=Li;
//            }

//            ii++;

//            if (Limin<Lij)
//            {
//                Lij=Limin;
//                minj=mini;
//            }

//        }
//        while ((ii<=g+3)||(ii<=num-1));

//        //Фильтрация в окне
//        if (mini!=(g))
//        {
//            H1b.clear();
//            Jump=mini;

//            double bY=0;

//            bY+=(sqrt(1-1/(alpha*alpha))*(MyXF.value(Jump-1)-MyXF.value(Jump)))/
//            (1-(((signalSigma*signalSigma)/(noiseSigma*noiseSigma))*(1-1/(alpha*alpha))*
//            (Hm1(Jump-1,Jump-1,LamVec,F)-Hm1(Jump-1,Jump,LamVec,F)-
//            Hm1(Jump,Jump-1,LamVec,F)+Hm1(Jump,Jump,LamVec,F))));

//            for (int i=0; i<num; i++)
//            {
//                H1b.insert(i,sqrt(1-1/(alpha*alpha))*(Hm1(i,Jump-1,LamVec,F)-Hm1(i,Jump,LamVec,F)));
//            }

//            do
//            {
//                //double mm1=MyXFN.value(c);
//                MyXFN[c]=(MyXF[c]+(noiseSigma*noiseSigma/(signalSigma*signalSigma)*bY*H1b[c]));
//                c++;
//            }
//            while ((c<=g+3)||(ii<=num-1));
//        } else {
//            do
//            {
//                //double mm1=MyXFN.value(c);
//                MyXFN[c]=MyXF[c];
//                c++;
//            }
//            while ((c<=g+3)||(ii<=num-1));
//        }

//    }
//}
