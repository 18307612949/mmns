# Multi-Machine-Network-System

#### Or, MMNS for short

By Brian Chrzanowski



## What is this MMNS thing that you've made

Quite simply, this project aims to become an interactive load-balancer. Load
balancers like [slurm](https://slurm.schedmd.com/) exist, and the market has
a couple that exist, but they behave much like Computing Associate's CA-7
product for IBM mainframes, in that a human schedules up a series of batch
jobs, and the load balancing system in between behaves like
[GNU parallel](https://www.gnu.org/software/parallel/), and simply takes your
command/scipt, and pushes it to however many computers you tell it to run on.


This program (client and server both) aims to create a load-balancing solution,
from (mostly) the ground up to help humans interact with cloud services,
with reliable latency and comfortable usability.


## So, what's so special?

Once completed, this will allow Nova Computing, and others to set up a
cloud solution for high performance computing. For example, we plan to have
the ability to change sessions and where your programs are rendered, and this
program is the glue that holds it all together.

## How do I use it?
The same binary can be reused as the client and the server. It behaves
similarly to `nc` in that when you provide it with the `-l` flag, it will
simply behave as a 'listener'.


There are two types of communication within this system.

1. A client will tell the listener something
2. A client will ask the listener for some data

What exactly the client asks for is based on an `unsigned short`. This is
described in, `data.h`.

## TODO
* JSON interface
* command line interface
* daemonization
* end-to-end encryption
