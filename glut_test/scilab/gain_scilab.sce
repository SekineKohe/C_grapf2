// set parameters
M = ;
m = ;
l = ;
J = ;
F = ;
c = ;
g = ;

// calculate linear system matrices
A = [0 0 0 0
     0 0 0 0
     0 0 0 0
     0 0 0 0
B = [0
     0
     0
     0];
 
// set weight matrices of linear quadratic regulartor
Q = diag([1 1 1 1]);
R = 1;

// call lqr and compute optimal gain
C = [sqrt(Q); zeros(1,4)];
D = [zeros(4,1); sqrt(R)]
[K, P] = lqr(syslin('c',A,B,C,D));

