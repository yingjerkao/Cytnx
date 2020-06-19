import numpy as np
import cytnx
from cytnx import cytnx_extension as cyx

"""
Reference: https://www.tensors.net
Author: Yu-Hsueh Chen, Kai-Hsin Wu 
"""

def Projector(psi, L, M1, M2, R):
    ''' psi is Tensor, while L,M1,M2,R are CyTensor.
    Return: h|psi> (Tensor)'''
    psi_p = cyx.CyTensor(psi,0) ## share memory, no copy
    psi_p.reshape_(L.shape()[1],M1.shape()[2],M2.shape()[2],R.shape()[1])
    anet = cyx.Network("projector.net")
    anet.PutCyTensors(["psi","L","M1","M2","R"],[psi_p,L,M1,M2,R]);
    H_psi = anet.Launch(optimal=True).get_block_() # get_block_ without copy
    H_psi.flatten_() # only change meta, without copy.
    psi.flatten_() ## this just in case psi is something shared. 
    return H_psi


def eig_Lanczos(psivec, linFunct, functArgs, maxit=2, krydim=4):
    """ Lanczos method for finding smallest algebraic eigenvector of linear \
    operator defined as a function"""
    #print(eig_Lanczos)

    psi_columns = cytnx.zeros([len(psivec), krydim + 1])
    krylov_matrix = cytnx.zeros([krydim, krydim])
    for ik in range(maxit):
        norm = max(psivec.Norm().item(), 1e-16)
        psi_columns[:, 0] = psivec / norm
        for ip in range(1, krydim + 1):


            psi_columns[:, ip] = linFunct(psi_columns[:, ip - 1], *functArgs)
            for ig in range(ip):
                krylov_matrix[ip - 1, ig] = cytnx.linalg.Dot(psi_columns[:, ip], psi_columns[:, ig])
                krylov_matrix[ig, ip - 1] = krylov_matrix[ip - 1, ig]

            for ig in range(ip):
                # print(cytnx.linalg.Dot(psi_columns[:, ig], psi_columns[:, ip]))
                vp = psi_columns[:, ip];
                vg = psi_columns[:, ig]
                vp = vp - cytnx.linalg.Dot(vg, vp).item()*vg;

                # print('psi_columns[:,ip].reshape(-1).Norm().item() = ', psi_columns[:,ip].reshape(-1).Norm().item())
                norm =  max(vp.Norm().item(), 1e-16)
                psi_columns[:, ip] = vp / norm ## only access set() once!! 

        [energy, psi_columns_basis] = cytnx.linalg.Eigh(krylov_matrix)
        psivec = cytnx.linalg.Matmul(psi_columns[:, :krydim],psi_columns_basis[:, 0].reshape(krydim,1)).flatten()

    norm = psivec.Norm().item()
    psivec = psivec / norm
    gs_energy = energy[0].item()
    return psivec, gs_energy



##### Set bond dimensions and simulation options
chi = 32;
Nsites = 20;
numsweeps = 4 # number of DMRG sweeps
maxit = 2 # iterations of Lanczos method
krydim = 4 # dimension of Krylov subspace

## Initialiaze MPO 
##>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
d = 2
s = 0.5
sx = cytnx.physics.spin(0.5,'x')
sy = cytnx.physics.spin(0.5,'y')
sp = sx+1j*sy
sm = sx-1j*sy
eye = cytnx.eye(d)
M = cytnx.zeros([4, 4, d, d])
M[0,0] = M[3,3] = eye
M[0,1] = M[2,3] = 2**0.5*sp.real()
M[0,2] = M[1,3] = 2**0.5*sm.real()
M = cyx.CyTensor(M,0)

L0 = cyx.CyTensor(cytnx.zeros([4,1,1]),0) #Left boundary
R0 = cyx.CyTensor(cytnx.zeros([4,1,1]),0) #Right boundary
L0.get_block_()[0,0,0] = 1.; R0.get_block_()[3,0,0] = 1.

## Init MPS train
#   
#   0-A[0]-2    2-A[1]-4    4-A[2]-6  ...  2k-A[k]-2k+2
#      |           |           |               |
#      1           3           5              2k+1
#
##>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
A = [None for i in range(Nsites)]
A[0] = cyx.CyTensor(cytnx.random.normal([1, d, min(chi, d)], 0., 1.),2)
for k in range(1,Nsites):
    dim1 = A[k-1].shape()[2]; dim2 = d;
    dim3 = min(min(chi, A[k-1].shape()[2] * d), d ** (Nsites - k - 1));
    A[k] = cyx.CyTensor(cytnx.random.normal([dim1, dim2, dim3],0.,1.),2)
    A[k].set_labels([2*k,2*k+1,2*k+2])

## Put in the left normalization form and calculate transfer matrices LR
#LR[0]:        LR[1]:            LR[2]:
#
#   -----      -----A[0]---     -----A[1]---
#   |          |     |          |     |
#  ML----     LR[0]--M-----    LR[1]--M-----      ......
#   |          |     |          |     |
#   -----      -----A*[0]--     -----A*[1]--
#
#
# L_AMAH.net file is used to contract the LR[i]
##>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
LR = [None for i in range(Nsites+1)]
LR[0]  = L0
LR[-1] = R0

for p in range(Nsites - 1):
    s, A[p] ,vt = cyx.xlinalg.Svd(A[p])
    A[p+1] = cyx.Contract(cyx.Contract(s,vt),A[p+1])
    #A[p+1].print_diagram()
    #A[p].print_diagram()
    anet = cyx.Network("L_AMAH.net")
    anet.PutCyTensors(["L","A","A_Conj","M"],[LR[p],A[p],A[p].Conj(),M],is_clone=False);
    LR[p+1] = anet.Launch(optimal=True);
            
_,A[-1] = cyx.xlinalg.Svd(A[-1],is_U=True,is_vT=False) ## last one.


## DMRG sweep
## Now we are ready for sweeping procedure!
##>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Ekeep = []

for k in range(1, numsweeps+2):
    
    # a. Optimize from right-to-left:
    # psi:                   Projector:
    # 
    #   --A[p]--A[p+1]--s--              --         --
    #      |       |                     |    | |    |
    #                                   LR[p]-M-M-LR[p+1]
    #                                    |    | |    |
    #                                    --         --
    # b. Transfer matrix from right to left :
    #  LR[-1]:       LR[-2]:            
    #
    #      ---          ---A[-1]---         
    #        |               |    | 
    #      --MR         -----M--LR[-1]   ......
    #        |               |    |
    #      ---          ---A*[-1]--
    #
    # c. For Right to Left, we want A's to be in shape
    #            -------------      
    #           /             \     
    #  virt ____| chi     chi |____ virt
    #           |             |     
    #  phys ____| 2           |        
    #           \             /     
    #            -------------      

    for p in range(Nsites-2,-1,-1): 
        #print(p)
        dim_l = A[p].shape()[0];
        dim_r = A[p+1].shape()[2];

        psi = cyx.Contract(A[p],A[p+1]) ## contract
        lbl = psi.labels() ## memorize label
        psi_T = psi.get_block_(); psi_T.flatten_() ## flatten to 1d
        psi_T, Entemp = eig_Lanczos(psi_T, Projector, (LR[p],M,M,LR[p+2]), maxit, krydim);
        psi_T.reshape_(dim_l,d,d,dim_r) ## convert psi back to 4-leg form 
        psi = cyx.CyTensor(psi_T,2);    
        psi.set_labels(lbl);
        Ekeep.append(Entemp);
        
        new_dim = min(dim_l*d,dim_r*d,chi)

        s,A[p],A[p+1] = cyx.xlinalg.Svd_truncate(psi,new_dim)
        s = s/s.get_block_().Norm().item()
        A[p] = cyx.Contract(A[p],s) ## absorb s into next neighbor

        # update LR from right to left:
        anet = cyx.Network("R_AMAH.net")
        anet.PutCyTensors(["R","B","M","B_Conj"],[LR[p+2],A[p+1],M,A[p+1].Conj()],is_clone=False)
        LR[p+1] = anet.Launch(optimal=True)
        
        print('Sweep[r->l]: %d/%d, Loc:%d,Energy: %f'%(k,numsweeps,p,Ekeep[-1]))

    A[0].set_rowrank(1)
    _,A[0] = cyx.xlinalg.Svd(A[0],is_U=False, is_vT=True)


    # a.2 Optimize from left-to-right:
    # psi:                   Projector:
    # 
    #   --A[p]--A[p+1]--s--              --         --
    #      |       |                     |    | |    |
    #                                   LR[p]-M-M-LR[p+1]
    #                                    |    | |    |
    #                                    --         --
    # b.2 Transfer matrix from left to right :
    #  LR[0]:       LR[1]:                   
    #
    #      ---          ---A[0]---                 
    #      |            |    |     
    #      L0-         LR[0]-M----    ......
    #      |            |    |
    #      ---          ---A*[0]--
    #
    # c.2 For Left to Right, we want A's to be in shape
    #            -------------      
    #           /             \     
    #  virt ____| chi     2   |____ phys
    #           |             |     
    #           |        chi  |____ virt        
    #           \             /     
    #            -------------      

    for p in range(Nsites-1):
        dim_l = A[p].shape()[0]
        dim_r = A[p+1].shape()[2]
        
        psi = cyx.Contract(A[p],A[p+1]) ## contract
        lbl = psi.labels() ## memorize label
        psi_T = psi.get_block_(); psi_T.flatten_() ## flatten to 1d
        psi_T, Entemp = eig_Lanczos(psi_T, Projector, (LR[p],M,M,LR[p+2]), maxit, krydim);
        psi_T.reshape_(dim_l,d,d,dim_r)## convert psi back to 4-leg form 
        psi = cyx.CyTensor(psi_T,2); psi.set_labels(lbl);
        Ekeep.append(Entemp);
        
        new_dim = min(dim_l*d,dim_r*d,chi)

        s,A[p],A[p+1] = cyx.xlinalg.Svd_truncate(psi,new_dim)
        s = s/s.get_block_().Norm().item()
        A[p+1] = cyx.Contract(s,A[p+1]) ## absorb s into next neighbor.

        anet = cyx.Network("L_AMAH.net")
        anet.PutCyTensors(["L","A","A_Conj","M"],[LR[p],A[p],A[p].Conj(),M],is_clone=False);
        LR[p+1] = anet.Launch(optimal=True);

        print('Sweep[l->r]: %d of %d, Loc: %d,Energy: %f' % (k, numsweeps, p, Ekeep[-1]))

    A[-1].set_rowrank(2)
    _,A[-1] = cyx.xlinalg.Svd(A[-1],is_U=True,is_vT=False) ## last one.




#### Compare with exact results (computed from free fermions)
from numpy import linalg as LA
import matplotlib.pyplot as plt
H = np.diag(np.ones(Nsites-1),k=1) + np.diag(np.ones(Nsites-1),k=-1)
D = LA.eigvalsh(H)
EnExact = 2*sum(D[D < 0])

##### Plot results
plt.figure(1)
plt.yscale('log')
plt.plot(range(len(Ekeep)), np.array(Ekeep) - EnExact, 'b', label="chi = 32", marker = 'o')
plt.legend()
plt.title('DMRG for XX model')
plt.xlabel('Update Step')
plt.ylabel('Ground Energy Error')
plt.show()




