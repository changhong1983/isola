
      subroutine manidataCUT(x,rr)

c cutting output after 6000dt !!!!!!
c Manipulating the data. Input data x, output data also x (re-written).
c Power of the data rr.

      dimension x(8192,21,3)
      dimension ntm(21)
      logical stat(21)


      common /NUMBERS/ nr,ntim,nmom,isubmax,ifirst,istep,ilast,
     *                 f1,f2,f3,f4,dt

      common /ST/ stat,ntm

c *******************************************************************
c *************MANIPULATING OBSERVED DATA****************************
c *******************************************************************

      do ir=1,nr
        do itim=1,ntim
            x(itim,ir,1)=0.
            x(itim,ir,2)=0.
            x(itim,ir,3)=0.
        enddo
      enddo

c
c     READING velocity SEISMOGRAMS TO BE INVERTED (=DATA)
c

      do ir=1,nr
        do itim=1,ntim
        nfile=1000+1*ir
           read(nfile,*) time,
     *      x(itim,ir,1),x(itim,ir,2),x(itim,ir,3)
        enddo
      enddo


c
c     FILTERING THE DATA 
c
      do ir=1,nr
        do icom=1,3
         call filter(f1,f2,f3,f4,dt,x(1,ir,icom))
        enddo
      enddo

	  zero=0.
      do ir=1,nr          ! output of filtered input data
      nfile=2000+1*ir
        do itim=1,ntim
        time=float(itim-1)* dt
		if(itim.le.6000) then
        write(nfile,'(4(1x,e12.6))') time,
     *       x(itim,ir,1), x(itim,ir,2), x(itim,ir,3)
	    else
        write(nfile,'(4(1x,e12.6))') time,zero,zero,zero
        endif
   	    enddo
	   enddo
    

c
c     COMPUTING 'POWER' OF THE original DATA (the USED stations ONLY !)
c

      rr=0.
      do icom=1,3                                       !icom=seismo comp.
        do ir=1,nr                                      !  ir=station
        if(stat(ir)) then
          do itim=1,ntim                                !itim=time
          rr=rr+x(itim,ir,icom)*x(itim,ir,icom)
          enddo
        endif
        enddo
      enddo
      rr=rr*dt

      RETURN
      END

