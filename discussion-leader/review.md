# Review

## Info

Reviewer Name: August Pokorak, Tom Pan
Paper Title: Disco: Running Commodity Operating Systems on Scalable Multiprocessors

## Summary
Disco is a virtual machine monitor that was designed to be run on large scale shared-memory multiprocessor systems, which had recently become widely accessible. Existing commodity OSes can easily be run on top of disco and can reap the benefits of the system's scale.

### Problem and why we care
The aforementioned large-scale shared memory, multiprocessor systems were desirable because of the possibility of scaling performance; however, in practice obtaining significant performance increases in these systems was limited by the design and modification of OSes. Specialized OSes for these systems were time consuming to develop and lacked the support and compatitibility of commodity OSes (and were often buggy/unstable).

### Gap in current approaches
Systems programmers can be spared from having to substantially redesign OSes to work on these new systems by instead running multiple OSes on a system using a VMM, and having the VMM address certain low-level details. However, prior to Disco VMMs were not widely used due to a number of established issues, such as significant performance and memory overheads

### Hypothesis, Key Idea, or Main Claim
A VMM that simultatiously runs multiple (slightly modified) OSes on recent large scale systems can capture the performance benefits of the hardware improvements without needing systems programmers to develop full custom OSes.  Furthermore, modifications to the underlying mechanisms of the VMM can improve performance and allow sharing of system resources between VMs.

### Method for Proving the Claim
They prove their claim by implementing their Disco VMM and benchmarking it in simulation against alternative options.  They describe their VMM implementation in terms of 3 main areas - CPU, memory, and I/O.

### Method for evaluating
They evaluate their solution based on the quantitative benchmarks provided in the paper, such as determining the memory load and scalability on various workloads.  Overall, they demonstrate relatively low overheads, reduced memory footprints, and scalability on a simulated multiprocessor system (but they don't seem to make any direct comparisons with "traditonal" VMMs)

### Contributions: what we take away
Virtualization is a powerful tool that can do a lot more than just provide isolation, in this case it is used to boost compatibility and (perhaps more surpisingly) scaling performance.  The VMM approach developed in this paper seems to have greatly informed the design of what came to be known as "hypervisors", which have been in use ever since.

## Pros (3-6 bullets)
- Able to use existing commodity OSes without significant modifications
- The Virtual Machine Monitor is relatively simple, with much fewer lines of code than true OSes (easy to develop and maintain)
- Captures performance benefits of large-scale multiprocessor systems

## Cons (3-6 bullets)
- Virtualization overheads still exist, and so may not capture as much performance benefits as a purpose-made large-scale OS
- Their initial implementation is tightly coupled to hardware (in terms of necessary changes to the OSes)
- Less complete isolation of virtual machines than previous VMM's due to sharing of memory
- Didn't directly compare to performance overheads of traditional VMM's

### What is your analysis of the proposed?

Summarize and justify what your evaluation of the paper is. 

This paper seems prescient, published in 1997 it sets the stage for the cloud computing boom two decades later, with the ability to easily rent parts of servers to run VMs and the distributed approach to computing that comes with it. Time has shown that the idea of running OSes on parts of a larger system is one which people are eager to pay for.  And the descriptions of their mechanisms and evaluation are mostly clear and easy to understand.  However, we would like them to have some discussion about the possible tradeoffs of their novel VMM mechanisms compared to the traditional, fully isolated case (as well as a direct performance overhead comparison on their workloads).  It is also true that some of their methods for decreasing VMM overheads are modifications of existing OSes, rather than changes in the VMM implementation.

## Details Comments, Observations, Questions

I had no idea that the concept of VMM's was developed so long ago, before this paper.

Is it not possible to provide a still simple mechanism for handling privileged code execution with less overhead?

Are VMM's similar in concept to microkernels?  Do they work better in practice?

Should existing OSes have to modified in any fashion in order to run efficiently in a VMM?

Don't exactly understand the use cases for replicating and moving the "physical" memory pages of OSes in machine memory.  Is this inherent to "non-uniform" memory systems?  And what exactly does non-uniform memory even mean?

Why is it beneficial to have some parts of the kernel address space be directly mapped to hardware addresses (bypassing the TLB)?

Legends in their figures are somewhat difficult to understand




