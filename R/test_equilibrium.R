################################################################################
##
##   Copyright (C) 2015 - 2017 the TraME Team:
##      Alfred Galichon
##      Keith O'Hara
##
##   This file is part of TraME.
##
##   TraME is free software: you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as published by
##   the Free Software Foundation, either version 2 of the License, or
##   (at your option) any later version.
##
##   TraME is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public License
##   along with TraME. If not, see <http://www.gnu.org/licenses/>.
##
################################################################################

test_ipfp <- function(seed=777, nbX=18, nbY=5)
{
    noSingles = (nbX == nbY)
    set.seed(seed)
    tm = proc.time()
    #
    message('*===================   Start of test_ipfp   ===================*\n')
    #

    sigma = 1

    n = rep(1,nbX)
    m = rep(1,nbY)

    alpha = matrix(runif(nbX*nbY),nbX,nbY)
    gamma = matrix(runif(nbX*nbY),nbX,nbY)

    lambda = 1 + matrix(runif(nbX*nbY),nbX,nbY)
    zeta = 1 + matrix(runif(nbX*nbY),nbX,nbY)

    phi = alpha + gamma

    lambda_LTU = lambda/(lambda + zeta)
    phi_LTU = (lambda*alpha + zeta*gamma) / (lambda + zeta)

    #

    mfe_geo_obj <- new(mfe_geo)
    mfe_geo_obj$build(n,m,phi,sigma,FALSE)

    mfe_cd_obj <- new(mfe_cd)
    mfe_cd_obj$build(n,m,lambda_LTU,phi_LTU,sigma,FALSE)

    mfe_min_obj <- new(mfe_min)
    mfe_min_obj$build(n,m,alpha,gamma,sigma,FALSE)

    #

    sol_geo <- mfe_geo_obj$solve("ipfp")
    message("Solution of TU-logit problem using ipfp:")
    print(c(sol_geo$mu))
    message("")

    sol_cd <- mfe_cd_obj$solve("ipfp")
    message("Solution of LTU-logit problem using parallel ipfp:")
    print(c(sol_cd$mu))
    message("")

    if (!noSingles) {
        sol_min <- mfe_min_obj$solve("ipfp")
        message("Solution of NTU-logit problem using ipfp:")
        print(c(sol_min$mu))
    }
    
    #

    time <- proc.time() - tm
    message(paste0('\nEnd of test_ipfp. Time elapsed = ', round(time["elapsed"],5), 's.\n'))

    #

    return(c(sol_geo$mu,sol_cd$mu))
}


test_nodal_newton <- function(seed=777, nbX=17, nbY=15, nbDraws=1e3)
{
    set.seed(seed)
    tm = proc.time()
    #
    message('*===================   Start of test_nodalNewton   ===================*\n')
    #

    sigma = 1

    n = rep(1,nbX)
    m = rep(1,nbY)

    phi = 1 + matrix(runif(nbX*nbY),nbX,nbY)
    alpha = matrix(runif(nbX*nbY),nbX,nbY)
    gamma = matrix(runif(nbX*nbY),nbX,nbY)

    #

    mfe_geo_obj <- new(mfe_geo)
    mfe_geo_obj$build(n,m,phi,sigma,FALSE)

    mfe_min_obj <- new(mfe_min)
    mfe_min_obj$build(n,m,alpha,gamma,sigma,FALSE)

    #

    r1 = ipfp(mfe_geo_obj)
    r1bis = nodal_newton(mfe_geo_obj)

    message("Solution of TU-logit:")
    message("mu_x0 using (i) IPFP and (ii) nodalNewton:")
    print(r1$mu_x0[1:min(nbX,10)])
    print(r1bis$mu_x0[1:min(nbX,10)])

    #

    r2 = ipfp(mfe_min_obj)
    r2bis = nodal_newton(mfe_min_obj)

    message("Solution of NTU-logit:")
    message("mu using (i) IPFP and (ii) nodalNewton:")
    print(r2$mu_x0[1:min(nbX,10)])
    print(r2bis$mu_x0[1:min(nbX,10)])
    
    #

    time = proc.time() - tm
    message(paste0('\nEnd of test_nodalNewton. Time elapsed = ', round(time["elapsed"],5), 's.\n')) 
  
    #
    
    ret <- c(r1$mu,r1bis$mu,r1$U,r1bis$U,r1$V,r1bis$V,r2$mu,r2bis$mu,r2$U,r2bis$U,r2$V,r2bis$V)
    return(ret)
}


test_arc_newton <- function(seed=777, nbX=5, nbY=3, nbDraws=1e3)
{
    set.seed(seed)
    tm = proc.time()
    #
    message('*===================   Start of test_arcNewton   ===================*\n')
    #

    n <- rep(1,nbX)
    m <- rep(1,nbY)
    
    phi <- matrix(runif(nbX*nbY),nrow=nbX)
    
    #

    mfe_geo_obj <- new(mfe_geo)
    mfe_geo_obj$build(n,m,phi,1.0,FALSE)

    dse_logit_obj_TU <- new(dse_logit_tu)
    dse_logit_obj_TU$build(n,m,phi,FALSE)

    r1 = ipfp(mfe_geo_obj)
    r1bis = arc_newton(dse_logit_obj_TU)

    #
    
    message("Solution of TU-logit:")
    message("mu using (i) IPFP and (ii) arcNewton:")
    print(r1$mu)
    print(r1bis$mu)
    #
    message("U using (i) IPFP and (ii) arcNewton:")
    print(c(r1$U)[1:min(5,nbX*nbY)])
    print(c(r1bis$U)[1:min(5,nbX*nbY)])
    #
    message("V using (i) IPFP and (ii) arcNewton:")
    print(c(r1$V)[1:min(5,nbX*nbY)])
    print(c(r1bis$V)[1:min(5,nbX*nbY)])  
    #
    time = proc.time() - tm
    message(paste0('\nEnd of test_arcNewton. Time elapsed = ', round(time["elapsed"],5), 's.\n')) 
    #
    ret <- c(r1$mu,r1bis$mu,r1$U,r1bis$U,r1$V,r1bis$V)
    return(ret)
}

test_max_welfare = function(seed=777, nbX=5, nbY=3, nbDraws=1e3)
{
    set.seed(seed)
    tm = proc.time()
    #
    message('*===================   Start of test_maxWelfare   ===================*\n')
    #
    
    n <- rep(1,nbX)
    m <- rep(1,nbY)
    
    phi <- matrix(runif(nbX*nbY),nrow=nbX)
    
    #

    mfe_geo_obj <- new(mfe_geo)
    mfe_geo_obj$build(n,m,phi,1.0,FALSE)

    dse_logit_obj_TU <- new(dse_logit_tu)
    dse_logit_obj_TU$build(n,m,phi,FALSE)

    r1 = ipfp(mfe_geo_obj)
    r1bis = max_welfare(dse_logit_obj_TU)
    
    #
    message("Solution of TU-logit:")
    #
    message("mu using (i) IPFP and (ii) maxWelfare:")
    print(r1$mu)
    print(r1bis$mu)
    #
    message("U using (i) IPFP and (ii) maxWelfare:")
    print(c(r1$U)[1:min(5,nbX*nbY)])
    print(c(r1bis$U)[1:min(5,nbX*nbY)])
    #
    message("V using (i) IPFP and (ii) maxWelfare:")
    print(c(r1$V)[1:min(5,nbX*nbY)])
    print(c(r1bis$V)[1:min(5,nbX*nbY)])
    message("")

    #
    # RSC

    rsc_G <- new(rsc)
    rsc_H <- new(rsc)

    zetaG = matrix(1,nbX,1) %*% matrix(runif(nbY+1),1,nbY+1)
    zetaH = matrix(1,nbY,1) %*% matrix(runif(nbX+1),1,nbX+1)
    
    rsc_G$build_beta(zetaG,2,2)
    rsc_H$build_beta(zetaH,2,2)

    m2 <- new(dse_rsc_tu)
    m2$build(n,m,phi,rsc_G,rsc_H,FALSE)

    m2Sim <- new(dse_empirical_tu)
    m2Sim$build(n,m,phi,rsc_G,rsc_H,FALSE)
    
    r2 = max_welfare(m2)
    r2Sim = cupids_lp(m2Sim)
    
    #
    message("Solution of TU-RSCbeta:")
    #
    message("val using (i) LP and (ii) maxWelfare:")
    print(r2Sim$val)
    print(r2$val)
    #
    message("mu using (i) LP and (ii) maxWelfare:")
    print(r2Sim$mu)
    print(r2$mu)
    #
    message("U using (i) LP and (ii) maxWelfare:")
    print(c(r2Sim$U)[1:min(5,nbX*nbY)])
    print(c(r2$U)[1:min(5,nbX*nbY)])
    #
    message("V using (i) LP and (ii) maxWelfare:")
    print(c(r2Sim$V)[1:min(5,nbX*nbY)])
    print(c(r2$V)[1:min(5,nbX*nbY)])  
    #
    time = proc.time() - tm
    message(paste0('\nEnd of test_maxWelfare. Time elapsed = ', round(time["elapsed"],5), 's.\n'))
    #
    ret <- c(r1$mu,r1bis$mu,r1$U,r1bis$U,r1$V,r1bis$V,r2Sim$val,r2$val,r2Sim$mu,r2$mu,r2Sim$U,r2$U,r2Sim$V,r2$V)
    return(ret)
}

test_jacobi <- function(nbDraws=1E3, seed=777, extensiveTesting = FALSE)
{
    set.seed(seed)
    ptm = proc.time()
    #
    message('*===================   Start of test_jacobi   ===================*\n')
    #

    alpha = matrix(c(1.6, 3.2, 1.1, 2.9, 1.0, 3.1),nrow=2,byrow=T)
    gamma = matrix(c(1.6, 3.2, 1.1, 2.9, 1.0, 3.1),nrow=2,byrow=T)
    muhat = matrix(c(1, 3, 1, 2, 1, 3), nrow=2, byrow=T)

    phi = alpha + gamma
    
    n = c(1.2*apply(muhat,1,sum))
    m = c(1.3*apply(t(muhat),1,sum))

    #

    m1 <- new(dse_logit_tu)
    m1$build(n,m,phi,FALSE)

    r1_jacobi = jacobi(m1)

    message("Solution of TU-logit problem using Jacobi:")
    print(c(r1_jacobi$mu))
    message("")

    #

    m2 <- new(dse_logit_ntu)
    m2$build(n,m,alpha,gamma,FALSE)

    r2_jacobi = jacobi(m2)
    
    message("Solution of NTU-logit problem using Jacobi:")
    print(c(r2_jacobi$mu))
    
    #

    # if(extensiveTesting==TRUE){
    #     nbX = length(n)
    #     nbY = length(m)
        
    #     logitM = build_logit(nbX,nbY)
    #     logitW = build_logit(nbY,nbX)
        
    #     logitSimM = simul(logitM,nbDraws,seed)
    #     logitSimW = simul(logitW,nbDraws,seed)
    #     #
    #     m2Sim = build_market_NTU_general(n,m,alpha,gamma,logitSimM,logitSimW)
    #     r2Sim_jacobi = jacobi(m2Sim)
    #     #
    #     message("\nSolution of NTU-logitSim problem using Jacobi:")
    #     print(c(r2Sim_jacobi$mu))
    # }

    #

    time = proc.time()-ptm
    message(paste0('\nEnd of test_jacobi. Time elapsed = ', round(time["elapsed"],5), 's.\n'))
    #
    ret <- c(r1_jacobi$mu,r2_jacobi$mu)
    return(ret) 
}

test_darum <- function(nbDraws=1E3,seed=777)
{
    set.seed(seed)
    ptm = proc.time()
    #
    message('*===================   Start of test_darum   ===================*\n')
    #

    alpha = matrix(c(1.6, 3.2, 1.1, 2.9, 1.0, 3.1),nrow=2,byrow=T)
    gamma = matrix(c(1.6, 3.2, 1.1, 2.9, 1.0, 3.1),nrow=2,byrow=T)
    muhat = matrix(c(1, 3, 1, 2, 1, 3), nrow=2, byrow=T)
    
    n = c(1.2*apply(muhat,1,sum))
    m = c(1.3*apply(t(muhat),1,sum))
 
    #

    m1 <- new(dse_logit_ntu)
    m1$build(n,m,alpha,gamma,FALSE)

    r1 = darum(m1)
    
    message("Solution of NTU-logit problem using darum:")  
    print(c(r1$mu))
    message("")

    #

    arums_G = m1$get_arums_G()
    arums_H = m1$get_arums_H()

    m1Sim <- new(dse_empirical_ntu)
    m1Sim$build(n,m,alpha,gamma,arums_G,arums_H,FALSE)

    r1Sim = darum(m1Sim)

    message("Solution of NTU-logitSim problem using darum:")  
    print(c(r1Sim$mu))

    #
    
    time = proc.time() - ptm
    message(paste0('\nEnd of test_darum. Time elapsed = ', round(time["elapsed"],5), 's.\n'))
    #
    ret <- c(r1$mu,r1Sim$mu)
    return(ret)
}

test_cupids_lp <- function(nbX=5, nbY=3, nbDraws=1E3, seed=777)
{
    set.seed(seed)
    ptm = proc.time()
    #
    message('*===================   Start of test_cupidsLP   ===================*\n')
    #
    
    alpha = matrix(runif(nbX*nbY),nrow=nbX)
    gamma = matrix(runif(nbX*nbY),nrow=nbX)

    phi <- alpha+gamma
    
    n = rep(1,nbX)
    m = rep(1,nbY)
    
    logitM = new(logit)
    logitW = new(logit)

    logitM$build(nbX,nbY)
    logitW$build(nbY,nbX)
    
    m1Sim <- new(dse_empirical_tu)
    m1Sim$build(n,m,phi,logitM,logitW,FALSE)

    #

    r1SimSmart <- cupids_lp(m1Sim)
    
    message("Solution of TU-logitSim problem using LP:")
    print(c(r1SimSmart$mu))
    message("")

    #

    rho = 0.3

    probit_G <- new(probit)
    probit_H <- new(probit)

    probit_G$build(nbX,nbY,rho,TRUE)
    probit_G$unifCorrelCovMatrices()

    probit_H$build(nbY,nbX,rho,TRUE)
    probit_H$unifCorrelCovMatrices()

    mktTUProbit <- new(dse_empirical_tu)
    mktTUProbit$build(n,m,phi,probit_G,probit_H,FALSE)

    #

    rTUProbit <- cupids_lp(mktTUProbit)

    message("Solution of TU-Probit problem using LP:")
    print(c(rTUProbit$mu))

    #

    time = proc.time() - ptm
    message(paste0('\nEnd of test_cupidsLP. Time elapsed = ', round(time["elapsed"],5), 's.\n')) 
    
    #
    ret <- c(r1SimSmart$mu,rTUProbit$mu)
    return(ret)
}

test_oap_lp <- function(nbX=8,nbY=5,seed=777)
{
    set.seed(seed)
    ptm <- proc.time()
    #
    message('*===================   Start of test_plainOAP   ===================*\n')
    #

    n=rep(1,nbX)
    m=rep(1,nbY)
    
    alpha = matrix(runif(nbX*nbY),nrow=nbX)
    gamma = matrix(runif(nbX*nbY),nrow=nbX)

    phi <- alpha+gamma

    mkt <- new(dse_none_tu)
    mkt$build(n,m,phi,FALSE)

    res <- oap_lp(mkt)

    #

    message('u:')
    print(res$u)
    message('v:')
    print(res$v)

    #

    time <- proc.time()-ptm
    message(paste0('\nEnd of test_oapLP. Time elapsed = ', round(time["elapsed"],5), 's.\n'))
    #
    ret <- c(res$u,res$v)
}  

test_eap_nash <- function(nbX=8,nbY=5,seed=777,debugmode=FALSE)
{
    set.seed(seed)
    ptm <- proc.time()
    #
    message('*===================   Start of test_nashITU   ===================*\n')
    #

    n=rep(1,nbX)
    m=rep(1,nbY)
    
    alpha = matrix(runif(nbX*nbY),nrow=nbX)
    gamma = matrix(runif(nbX*nbY),nrow=nbX)
    lambda = matrix(1+runif(nbX*nbY),nrow=nbX)
    zeta = matrix(1+runif(nbX*nbY),nrow=nbX)

    lambda_LTU <- lambda/(lambda+zeta)
    phi_LTU <- (lambda*alpha + zeta*gamma)/(lambda + zeta)

    #

    mkt <- new(dse_none_ltu)
    mkt$build(n,m,lambda_LTU,phi_LTU,FALSE)
    
    #

    nash1 = eap_nash(mkt,TRUE)
    unash1 = nash1$u
    vnash1 = nash1$v
    #
    nash2 = eap_nash(mkt,FALSE)
    unash2 = nash2$u
    vnash2 = nash2$v
    #
    message("u[x] (upper and lower):")
    print(matrix(c(unash1,unash2),nrow=2,byrow=TRUE))
    message("v[y] (lower and upper):")
    print(matrix(c(vnash1,vnash2),nrow=2,byrow=TRUE)) 
    # print(c("Method","u1","u2","u3","v1","v2","v3","val"))
    # print(c("na1",round(c(unash1[1:3],vnash1[1:3]),2),round(sum(n*unash1)+sum(m*vnash1),4)))
    # print(c("na2",round(c(unash2[1:3],vnash2[1:3]),2),round(sum(n*unash2)+sum(m*vnash2),4)))
    # print("---------")
    #
    time <- proc.time() - ptm
    message(paste0('\nEnd of test_eapNash. Time elapsed = ', round(time["elapsed"],5), 's.\n'))
    #
    ret <- c(c(unash1,unash2),c(vnash1,vnash2))
    return(ret)
}  

tests_equilibrium = function(notifications=TRUE,nbDraws=1e3){
    ptm = proc.time()
    #
    res_darum       <- round(test_darum(nbDraws=nbDraws),5)
    res_ipfp        <- round(test_ipfp(),5)
    res_nodalNewton <- round(test_nodal_newton(),5)
    res_arcNewton   <- round(test_arc_newton(nbDraws=nbDraws),5)
    res_maxW        <- round(test_max_welfare(nbDraws=nbDraws),5)
    res_jacobi      <- round(test_jacobi(nbDraws=nbDraws),5)
    res_CLP         <- round(test_cupids_lp(nbDraws=nbDraws),5)
    res_oapLP       <- round(test_oap_lp(),5)
    res_nash        <- round(test_eap_nash(),5)
    
    res_all <- c(res_darum,res_ipfp,res_nodalNewton,res_arcNewton,res_maxW,res_jacobi,res_CLP,res_oapLP,res_nash)
    # MD5 checksum
    res_darum_md5       <- digest(res_darum,algo="md5")
    res_ipfp_md5        <- digest(res_ipfp,algo="md5")
    res_nodalNewton_md5 <- digest(res_nodalNewton,algo="md5")
    res_arcNewton_md5   <- digest(res_arcNewton,algo="md5")
    res_maxW_md5        <- digest(res_maxW,algo="md5")
    res_jacobi_md5      <- digest(res_jacobi,algo="md5")
    res_CLP_md5         <- digest(res_CLP,algo="md5")
    res_oapLP_md5       <- digest(res_oapLP,algo="md5")
    res_nash_md5        <- digest(res_nash,algo="md5")
    
    res_all_md5 <- digest(res_all,algo="md5")
    #
    time = proc.time() - ptm
    
    if (notifications) {
        message(paste0('All tests of Equilibrium completed. Overall time elapsed = ', round(time["elapsed"],5), 's.'))
    }
    #
    ret <- list(res_all_md5=res_all_md5,res_darum_md5=res_darum_md5,res_ipfp_md5=res_ipfp_md5,
                res_nodalNewton_md5=res_nodalNewton_md5,res_arcNewton_md5=res_arcNewton_md5,
                res_maxW_md5=res_maxW_md5,res_jacobi_md5=res_jacobi_md5,res_CLP_md5=res_CLP_md5,
                res_oapLP_md5=res_oapLP_md5,res_nash_md5=res_nash_md5)
    #
    return(ret)
}

