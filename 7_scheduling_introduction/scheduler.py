#! /usr/bin/env python

import sys
from optparse import OptionParser
import random
import operator
from typing import List, Union

parser = OptionParser()
parser.add_option(
    "-s",
    "--seed",
    default=0,
    help="the random seed",
    action="store",
    type="int",
    dest="seed",
)
parser.add_option(
    "-j",
    "--jobs",
    default=3,
    help="number of jobs in the system",
    action="store",
    type="int",
    dest="jobs",
)
parser.add_option(
    "-l",
    "--jlist",
    default="",
    help="instead of random jobs, provide a comma-separated list of run times",
    action="store",
    type="string",
    dest="jlist",
)
parser.add_option(
    "-m",
    "--maxlen",
    default=10,
    help="max length of job",
    action="store",
    type="int",
    dest="maxlen",
)
parser.add_option(
    "-p",
    "--policy",
    default="FIFO",
    help="sched policy to use: SJF, FIFO, RR",
    action="store",
    type="string",
    dest="policy",
)
parser.add_option(
    "-q",
    "--quantum",
    help="length of time slice for RR policy",
    default=1,
    action="store",
    type="int",
    dest="quantum",
)
parser.add_option(
    "-c",
    help="compute answers for me",
    action="store_true",
    default=False,
    dest="solve",
)

(options, args) = parser.parse_args()

random.seed(options.seed)

print("ARG policy", options.policy)
if options.jlist == "":
    print("ARG jobs", options.jobs)
    print("ARG maxlen", options.maxlen)
    print("ARG seed", options.seed)
else:
    print("ARG jlist", options.jlist)

print("")

print("Here is the job list, with the run time of each job: ")

joblist: List[List[Union[int, float]]] = []
if options.jlist == "":
    for jobnum in range(0, options.jobs):
        runtime = int(options.maxlen * random.random()) + 1
        joblist.append([jobnum, float(runtime)])
        print("  Job", jobnum, "( length = " + str(runtime) + " )")
else:
    jobnum = 0
    for runtime in options.jlist.split(","):
        joblist.append([jobnum, float(runtime)])
        jobnum += 1
    for job in joblist:
        print("  Job", job[0], "( length = " + str(job[1]) + " )")
print("\n")

if options.solve:
    print("** Solutions **\n")
    if options.policy == "SJF":
        joblist = sorted(joblist, key=operator.itemgetter(1))
        options.policy = "FIFO"

    if options.policy == "FIFO":
        thetime: float = 0
        print("Execution trace:")
        for job in joblist:
            print(
                "  [ time %3d ] Run job %d for %.2f secs ( DONE at %.2f )"
                % (thetime, job[0], job[1], thetime + job[1])
            )
            thetime += job[1]

        print("\nFinal statistics:")
        t = 0.0
        count = 0
        turnaroundSum = 0.0
        waitSum = 0.0
        responseSum = 0.0
        for tmp in joblist:
            jobnum = int(tmp[0])
            runtime = int(tmp[1])

            response = t
            turnaround = t + runtime
            wait = t
            print(
                "  Job %3d -- Response: %3.2f  Turnaround %3.2f  Wait %3.2f"
                % (jobnum, response, turnaround, wait)
            )
            responseSum += response
            turnaroundSum += turnaround
            waitSum += wait
            t += runtime
            count = count + 1
        print(
            "\n  Average -- Response: %3.2f  Turnaround %3.2f  Wait %3.2f\n"
            % (responseSum / count, turnaroundSum / count, waitSum / count)
        )

    if options.policy == "RR":
        print("Execution trace:")
        turnaround_times = {}
        response_times = {}
        lastran = {}
        wait_times = {}
        quantum = float(options.quantum)
        jobcount = len(joblist)
        for i in range(0, jobcount):
            lastran[i] = 0.0
            wait_times[i] = 0.0
            turnaround_times[i] = 0.0
            response_times[i] = -1

        runlist = []
        for e in joblist:
            runlist.append(e)

        thetime_ = 0.0
        while jobcount > 0:
            # print '%d jobs remaining' % jobcount
            job = runlist.pop(0)
            jobnum = int(job[0])
            runtime_ = float(job[1])
            if response_times[jobnum] == -1:
                response_times[jobnum] = int(thetime_)
            currwait = thetime_ - lastran[jobnum]
            wait_times[jobnum] += currwait
            if runtime_ > quantum:
                runtime_ -= quantum
                ranfor = quantum
                print(
                    "  [ time %3d ] Run job %3d for %.2f secs"
                    % (thetime_, jobnum, ranfor)
                )
                runlist.append([jobnum, runtime_])
            else:
                ranfor = runtime_
                print(
                    "  [ time %3d ] Run job %3d for %.2f secs ( DONE at %.2f )"
                    % (thetime_, jobnum, ranfor, thetime_ + ranfor)
                )
                turnaround_times[jobnum] = thetime_ + ranfor
                jobcount -= 1
            thetime_ += ranfor
            lastran[jobnum] = thetime_

        print("\nFinal statistics:")
        turnaroundSum = 0.0
        waitSum = 0.0
        responseSum = 0.0
        for i in range(0, len(joblist)):
            turnaroundSum += turnaround_times[i]
            responseSum += response_times[i]
            waitSum += wait_times[i]
            print("  Job %3d -- Response: %3.2f  Turnaround %3.2f  Wait %3.2f" % (i, response_times[i], turnaround_times[i], wait_times[i]))
        count = len(joblist)

        print(
            "\n  Average -- Response: %3.2f  Turnaround %3.2f  Wait %3.2f\n"
            % (responseSum / count, turnaroundSum / count, waitSum / count)
        )

    if options.policy != "FIFO" and options.policy != "SJF" and options.policy != "RR":
        print("Error: Policy", options.policy, "is not available.")
        sys.exit(0)
else:
    print("Compute the turnaround time, response time, and wait time for each job.")
    print("When you are done, run this program again, with the same arguments,")
    print("but with -c, which will thus provide you with the answers. You can use")
    print("-s <somenumber> or your own job list (-l 10,15,20 for example)")
    print("to generate different problems for yourself.")
    print("")
