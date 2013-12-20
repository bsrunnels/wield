#include "SurfaceIntegrate.h"
#include <stdio.h>

double SurfaceIntegrate(int order, 
			double Ca[4][4][4], double Cb[4][4][4], 
			Vector3d na3, Vector3d na1, Vector3d nb3, Vector3d nb1, 
			Vector6d alpha,
			//double alpha1, double alpha2, double alpha3, double beta1, double beta2, double beta3,
			double epsilon, double tol,
			string distribution,double selfenergyfactor)
{
  int type; 
  if (distribution=="gaussian") type = 0;
  else if (distribution=="cauchy") type = 1;
  else if (distribution=="fakecauchy") type = 2;
  else {cout << "Error: Distribution type not specified" << endl; throw(0);}
  double S;
  if (abs(na3.dot(na1)) > 1E-8) {cout << "NA3, NA1 must be orthogonal" << endl;throw(0);}
  if (abs(nb3.dot(nb1)) > 1E-8) {cout << "NB3, NB1 must be orthogonal" << endl;throw(0);}
  na3 = na3/na3.norm(); nb3 = nb3/nb3.norm();
  na1 = na1/na1.norm(); nb1 = nb1/nb1.norm();
  Vector3d na2 = na3.cross(na1); Vector3d nb2 = nb3.cross(nb1);
  S=0;
  for (int i=0;i<order;i++)
    for (int j=0;j<order;j++) 
      for (int k=0;k<order;k++) 
	for (int l=0;l<order;l++)
	  for (int m=0;m<order;m++)
	    for (int n=0;n<order;n++)
	      for (int p=0;p<2;p++)
		for (int q=0;q<2;q++)
		  for (int r=0;r<2;r++)
		    for (int s=0;s<2;s++)
		      for (int t=0;t<2;t++)
			for (int u=0;u<2;u++)
			  if ( (p+q+r+s+t+u)%2 == 0 ) // only do if p+q+r+s+t+u is even
			    if (abs(Ca[i][j][k]*Cb[l][m][n]) > tol) // skip terms with low coefficients
			      {
				double Ia1_1 = 2*(double)i*PI*na1[0]/alpha[0]; double Ia2_1 = 2*(double)i*PI*na2[0]/alpha[0]; 
				double Ja1_2 = 2*(double)j*PI*na1[1]/alpha[1]; double Ja2_2 = 2*(double)j*PI*na2[1]/alpha[1];
				double Ka1_3 = 2*(double)k*PI*na1[2]/alpha[2]; double Ka2_3 = 2*(double)k*PI*na2[2]/alpha[2];
				double Lb1_1 = 2*(double)l*PI*nb1[0]/alpha[3]; double Lb2_1 = 2*(double)l*PI*nb2[0]/alpha[3];
				double Mb1_2 = 2*(double)m*PI*nb1[1]/alpha[4]; double Mb2_2 = 2*(double)m*PI*nb2[1]/alpha[4];
				double Nb1_3 = 2*(double)n*PI*nb1[2]/alpha[5]; double Nb2_3 = 2*(double)n*PI*nb2[2]/alpha[5];

				int F[6] = {p, q, r, s, t, u};
				double a1[6] = {Ia1_1, Ja1_2, Ka1_3, Lb1_1, Mb1_2, Nb1_3};
				double a2[6] = {Ia2_1, Ja2_2, Ka2_3, Lb2_1, Mb2_2, Nb2_3};

				double factor;
				// if (i+j+k = 0) continue;
				// if (l+m+n = 0) continue;
				if ((i+j+k == 0) || (l+m+n == 0)) factor = selfenergyfactor;
				else factor = 1;
				//if ((i+j+k != 0) && (l+m+n != 0)) continue;
				

				
				if (type==0) // Gaussian
				  S += 
				    factor*
				    Ca[i][j][k]*Cb[l][m][n]
				    * Trig6Int(F, a1, epsilon)
				    * Trig6Int(F, a2, epsilon);
				else if (type==1) // Cauchy
				  S += 
				    factor*
				    Ca[i][j][k]*Cb[l][m][n]
				    * RS::Trig6Int_2(F,F,a1,a2,epsilon);
				else // Fake Cauchy
				  S += 
				    factor*
				    Ca[i][j][k]*Cb[l][m][n]
				    * RS::Trig6Int(F, a1, epsilon)
				    * RS::Trig6Int(F, a2, epsilon);

			      }
  return S;
}

double SurfaceIntegrate_epsilon(int order, 
				double Ca[4][4][4], double Cb[4][4][4], 
				Vector3d na3, Vector3d na1, Vector3d nb3, Vector3d nb1, 
				Vector6d alpha,
				double epsilon, double tol)
{

  double S;
  if (abs(na3.dot(na1)) > 1E-8) {cout << "NA3, NA1 must be orthogonal" << endl;throw(0);}
  if (abs(nb3.dot(nb1)) > 1E-8) {cout << "NB3, NB1 must be orthogonal" << endl;throw(0);}
  na3 = na3/na3.norm(); nb3 = nb3/nb3.norm();
  na1 = na1/na1.norm(); nb1 = nb1/nb1.norm();
  Vector3d na2 = na3.cross(na1); Vector3d nb2 = nb3.cross(nb1);
  S=0;
  for (int i=0;i<order;i++)
    for (int j=0;j<order;j++) 
      for (int k=0;k<order;k++) 
	for (int l=0;l<order;l++)
	  for (int m=0;m<order;m++)
	    for (int n=0;n<order;n++)
	      for (int p=0;p<2;p++)
		for (int q=0;q<2;q++)
		  for (int r=0;r<2;r++)
		    for (int s=0;s<2;s++)
		      for (int t=0;t<2;t++)
			for (int u=0;u<2;u++)
			  if ( (p+q+r+s+t+u)%2 == 0 ) // only do if p+q+r+s+t+u is even
			    if (abs(Ca[i][j][k]*Cb[l][m][n]) > tol) // skip terms with low coefficients
			      {
				double Ia1_1 = 2*(double)i*PI*na1[0]/alpha[0]; double Ia2_1 = 2*(double)i*PI*na2[0]/alpha[0]; 
				double Ja1_2 = 2*(double)j*PI*na1[1]/alpha[1]; double Ja2_2 = 2*(double)j*PI*na2[1]/alpha[1];
				double Ka1_3 = 2*(double)k*PI*na1[2]/alpha[2]; double Ka2_3 = 2*(double)k*PI*na2[2]/alpha[2];
				double Lb1_1 = 2*(double)l*PI*nb1[0]/alpha[3]; double Lb2_1 = 2*(double)l*PI*nb2[0]/alpha[3];
				double Mb1_2 = 2*(double)m*PI*nb1[1]/alpha[4]; double Mb2_2 = 2*(double)m*PI*nb2[1]/alpha[4];
				double Nb1_3 = 2*(double)n*PI*nb1[2]/alpha[5]; double Nb2_3 = 2*(double)n*PI*nb2[2]/alpha[5];

				int F[6] = {p, q, r, s, t, u};
				double a1[6] = {Ia1_1, Ja1_2, Ka1_3, Lb1_1, Mb1_2, Nb1_3};
				double a2[6] = {Ia2_1, Ja2_2, Ka2_3, Lb2_1, Mb2_2, Nb2_3};

				if (i+j+k == 0) continue;
				if (l+m+n == 0) continue;

				S += 
				  Ca[i][j][k]*Cb[l][m][n]
				  * (Trig6Int_epsilon(F, a1, epsilon) * Trig6Int(F, a2, epsilon) + 
				     Trig6Int(F, a1, epsilon)         * Trig6Int_epsilon(F, a2, epsilon));
			      }
  return S;
}


Vector6d DSurfaceIntegrate(int order, 
			   double Ca[4][4][4], double Cb[4][4][4], 
			   Vector3d na3, Vector3d na1, Vector3d nb3, Vector3d nb1, 
			   Vector6d alpha,
			   //double alpha1, double alpha2, double alpha3, double beta1, double beta2, double beta3,
			   double epsilon, double tol)
{
  double I1, I2;
  Vector6d DS, DI1, DI2;
  if (abs(na3.dot(na1)) > 1E-8) {cout << "NA3, NA1 must be orthogonal" << endl;throw(0);}
  if (abs(nb3.dot(nb1)) > 1E-8) {cout << "NB3, NB1 must be orthogonal" << endl;throw(0);}
  na3 = na3/na3.norm(); nb3 = nb3/nb3.norm();
  na1 = na1/na1.norm(); nb1 = nb1/nb1.norm();
  Vector3d na2 = na3.cross(na1); Vector3d nb2 = nb3.cross(nb1);
  DS*=0;
  for (int i=0;i<order;i++)
    for (int j=0;j<order;j++) 
      for (int k=0;k<order;k++) 
	for (int l=0;l<order;l++)
	  for (int m=0;m<order;m++)
	    for (int n=0;n<order;n++)
	      for (int p=0;p<2;p++)
		for (int q=0;q<2;q++)
		  for (int r=0;r<2;r++)
		    for (int s=0;s<2;s++)
		      for (int t=0;t<2;t++)
			for (int u=0;u<2;u++)
			  if ( (p+q+r+s+t+u)%2 == 0 ) // only do if p+q+r+s+t+u is even
			    if (abs(Ca[i][j][k]*Cb[l][m][n]) > tol) // skip terms with low coefficients
			      {
				double Ia1_1 = 2*(double)i*PI*na1[0]/alpha[0]; double Ia2_1 = 2*(double)i*PI*na2[0]/alpha[0]; 
				double Ja1_2 = 2*(double)j*PI*na1[1]/alpha[1]; double Ja2_2 = 2*(double)j*PI*na2[1]/alpha[1];
				double Ka1_3 = 2*(double)k*PI*na1[2]/alpha[2]; double Ka2_3 = 2*(double)k*PI*na2[2]/alpha[2];
				double Lb1_1 = 2*(double)l*PI*nb1[0]/alpha[3]; double Lb2_1 = 2*(double)l*PI*nb2[0]/alpha[3];
				double Mb1_2 = 2*(double)m*PI*nb1[1]/alpha[4]; double Mb2_2 = 2*(double)m*PI*nb2[1]/alpha[4];
				double Nb1_3 = 2*(double)n*PI*nb1[2]/alpha[5]; double Nb2_3 = 2*(double)n*PI*nb2[2]/alpha[5];

				int F[6] = {p, q, r, s, t, u};
				double a1[6] = {Ia1_1, Ja1_2, Ka1_3, Lb1_1, Mb1_2, Nb1_3};
				double a2[6] = {Ia2_1, Ja2_2, Ka2_3, Lb2_1, Mb2_2, Nb2_3};

				if (i+j+k == 0) continue;
				if (l+m+n == 0) continue;

				I1 = Trig6Int(F, a1, epsilon);
				I2 = Trig6Int(F, a2, epsilon);

				DI1 = DTrig6Int(F, a1, epsilon);
				DI2 = DTrig6Int(F, a2, epsilon);
				for (int ctr=0;ctr<6;ctr++)
				  {
				    DI1[ctr] *= - a1[ctr] / alpha[ctr]; // Chain
				    DI2[ctr] *= - a2[ctr] / alpha[ctr]; // rule
				  }

				DS += 
				  Ca[i][j][k]*Cb[l][m][n]
				  * (DI1*I2 + I2*DI2); // Product rule
			      }
return DS;
}