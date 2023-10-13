# Review

## Info

Reviewer Name: August Pokorak, Tom Pan
Paper Title: Disco: Running Commodity Operating Systems on Scalable Multiprocessors

## Summary
Disco is an OS that runs on large scale shared-memory multiprocessor systems which serves as a virtual machine monitor. Commodity OSes can run on top of disco and can reap the benefits of the system's scale.

### Problem and why we care
Large scale shared memory systems are desirable because of the possibility of scaling performance; however, OS design factors can impact how well the performance scales in practice.

### Gap in current approaches
In practice, the performance of these large-scale systems is limited by the design of commodity OSes. Meanwhile, specialized OSes for these systems are time consuming to develop and lack the support and compatitibility of commodity OSes.

### Hypothesis, Key Idea, or Main Claim
A virtual machine monitor OS simultatiously running multiple commodity OSes on these large scale systems can capture the performance benefits of the scale without needing a full custom OS.

### Method for Proving the Claim
They prove their claim by implementing Disco, their VM monitor OS and benchmarking it in simulation against alternative options.

### Method for evaluating
They evaluate their solution based on the quantitative benchmarks provided in the paper, such as determining the memory load and scalability on various workloads.

### Contributions: what we take away
Virtualization is a powerful tool that can do a lot more than just provide isolation, in this case it is used to boost compatibility and, more suropisingly, performance.

## Pros (3-6 bullets)
- Able to use commodity OS
- Monitor OS is relatively simple (easy to develop and maintain)
- Captures performance benefits of large-scale systems

## Cons (3-6 bullets)
- Virtualization overhead
- May not capture as much performance benefit as a purpose-made large-scale OS
- Tightly coupled to hardware

### What is your analysis of the proposed?

Summarize and justify what your evluation of the paper is. 

This paper seems prescient, published in 1997 it sets the stage for the cloud computing boom two decades later, with the ability to easily rent parts of servers to run VMs and the distributed approach to computing that comes with it. Time has shown that the idea of running commodity OSes on parts of a larger system is one which people are eager to pay for.

## Details Comments, Observations, Questions


