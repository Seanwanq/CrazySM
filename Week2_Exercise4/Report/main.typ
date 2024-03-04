
#import "@preview/tablex:0.0.8": *
#import "template.typ": *

#import "sourcerer.typ": *


#show: MSTemplate.with(
  reportName: "Homework Report 1",
  authorName: "Siyuan Wang 0485101, Linglan Li 4508459",
  courseName: "Modeling and Simulation",
  professor: "Prof. Dr. it. Marjolein Dijksta and Dr. Laura Filion",
  date: "February 27th, 2024",
  IsComplexEquationNumberingOn: true,
  ShowProfessorName: false
)

= Introduction
In the _Homework Report 1_ I will do the following things:

+ Write a C program to generate the data file of a cubic lattice.

+ Calculate the maximum volume fraction for cubic lattice.

+ Generate the data file of a face-centered cubic (FCC) lattice.

+ Calculate the maximum volume fraction of a FCC lattice and compare it with a cubic lattice.

+ Complete the `read_data()` function.

+ Complete the `move_particle()` function and implement displacement without considering overlapping.

+ Improve the `move_particle()` function by considering overlapping.

+ Visualize the model.

+ Do more research on `packing_fraction`.

= Implementation
== Build a cubic lattice <q1>
The implementation of the cubic lattice was written in `cubic_lattice.c`. Some code snippets are shown in @cq1.
The generated data file is `cubic.dat`. The first 6 lines are:
#code(lang: "cubic.dat",
```dat
3375
0.000000 15.000000
0.000000 15.000000
0.000000 15.000000
0.000000	0.000000	0.000000	1.000000
0.000000	0.000000	1.000000	1.000000
```
)
Here $N=3375$ because I let the length of the cubic be 15. That makes $15^3 = 3375$. The visualized cubic is shown in @cl 
#FigureBlock(
  captionAlign: "c")[
  #figure(
    placement: auto,
    image(
      "./figures/cubic1.png",
      width: 55%
    ),
    caption: [A Cubic Lattice.]
  ) <cl>
]

== Calculate the maximum volume fraction of a cubic lattice
To calculate the maximum volume fraction of a cubic lattice, we can refer to @fractioncl. As we can see, the sphere has occupied the maximum volume of the cubic, with the radius $r$. Meanwhile, the cubic has its side length $2r$.

#include "./figures/fraction_cl.typ"

Then we can get *the volume of the sphere*:
$
V_"sphere" = 4/3 pi r^3,
$<vs>
*the volume of the cubic*:
$
V_"cubic" = (2r)^3 = 8r^3,
$<vc>
*the maximum volume fraction*:
$
italic("fraction") = V_"sphere" / V_"cubic" = pi/6.
$<fsc>

== Build a FCC lattice<q3>
The implementation of the FCC lattice was written in `fcc_lattice.c`. Some code snippets are shown in @cq3. The generated data file is `fcc.dat`. The visualized FCC lattice is shown in @fccl.

#FigureBlock(
  captionAlign: "c",
)[
  #figure(
    placement: auto,
    image(
      "./figures/fcc1.png",
      width: 45%
    ),
    caption: [A FCC Lattice.]
  )<fccl>
]

== Calculate the maximum volume fraction of an FCC lattice
To calculate the maximum volume fraction of a FCC lattice, we can refer to @fractionfccl. In the 3 dimensional space, the maximum volume means there are 2 complete balls in the cubic.

#include "figures/fraction_fccl.typ"

Then we can get the volume of the spheres:
$
V_"sphere" = 2 times 4/3 pi r^3,
$ <vsfcc>
the volume of the cubic:
$
V_"cubic" = (2 sqrt(2) r)^3 = 16 sqrt(2) r^3.
$ <vcfcc>
Thus the maximum volume fraction is:
$
italic("fraction") = V_"sphere" / V_"cubic" = pi/(6 sqrt(2)).
$ <fsfcc>

Compared to the maximum volume fraction of a cubic lattice, we can see that the value is a little smaller, which means FCC lattices have lower space utilization.

== Complete the `read_data()` function <q5>
The code of `read_data()` imitates the structure of `write_data(int step)`, allowing it to correctly read the data in `coords_step0000000.dat`. The code snippet is shown in @cq5.

== Complete the `move_particle()` function without considering overlapping <q6>
The code of `move_particle()` is written in @cq6. For this function, overlapping is not detected, and it will always return 1. Periodic boundary condition is always applied.

Here are some outputs when the packing fraction equals 0.6 and $Delta x$ equals 0.1. From @overlap:parent we can see overlapping takes place in every figure. Thus we must consider some ways to fix this problem.

#FigureBlock(
  captionAlign: "c",
)[
  #figure(
    placement: none,
    caption: [Outputs when $"packing fraction" = 0.6$ and $Delta x = 0.1$.]
  )[
    #grid(
      columns: (1fr, 1fr, 1fr),
      gutter: 10pt
    )[
      #subfigure(
        image(
          "./figures/overlapstep100.png"
        ),
        caption: [$italic("step") = 100$.],
        lbl: <overlap:s100>
      )][
      #subfigure(
        image(
          "./figures/overlapstep1000.png"
        ),
        caption: [$italic("step") = 1000.$],
        lbl:<overlap:s1000>
      )][
        #subfigure(
          image(
            "./figures/overlapstep9000.png"
          ),
          caption: [$italic("step") = 9000$.],
          lbl:<overlap:s9000>
        )
      ]
    ]<overlap:parent>    
  ]

== Complete the `movo_particle()` function by considering overlapping <q7>
Now we consider the overlapping. By calculating the distance between a newly moved particle and other particles, we can know whether the movement should be accepted. The improved code is placed in @cq7.

According to the lecture note, a very small or very large value of $Delta x$ makes the simulation very inefficient. We should choose a $Delta x$ to make the acceptance to be around 50%. The lecture note suggests that in all cases, $Delta x$ should not be larger than half the box size.

Here are several examples of packing fraction and delta in @table1.

#figure(
  placement: auto,
  tablex(
    columns: 3,
    align: center+horizon,
    auto-vlines: false,
    [*Packing Fraction*], [*$Delta x$*], [*Average Acceptance*],
    [0.7], [0.006], [0.524040],
    [0.6], [0.03], [0.517964],
    [0.5], [0.05], [0.536721],
    [0.4], [0.08], [0.510905],
    [0.3], [0.15], [0.528917],
    [0.2], [0.3], [0.549655],
    [0.1], [1.0], [0.512143],
  ),
  kind: table,
  supplement: "Table",
  caption: [List of resonable $Delta x$.]
) <table1>

From @table1 we can see that the value of chosen $Delta x$ increases monotonically as the packing fraction decreases, which is consistent with the principle of crystal packing. When the packing fraction drops, crystal density decreases, causing atoms to be farther apart. With more space between atoms, the chance of moving any atom without overlap increases.

Hence, the $Delta x$ values also increase. This simple connection shows how changes in packing fraction impact crystal density and atomic movements.

#FigureBlock(
  captionAlign: "c",
)[
  #figure(
    placement: none,
    caption: [Outputs when $"packing fraction" = 0.6$ and $Delta x = 0.1$.]
  )[
    #grid(
      columns: (1fr, 1fr, 1fr),
      gutter: 10pt
    )[
      #subfigure(
        image(
          "./figures/noolstep100.png"
        ),
        caption: [$italic("step") = 100$.],
        lbl: <nool:s100>
      )][
      #subfigure(
        image(
          "./figures/noolstep500.png"
        ),
        caption: [$italic("step") = 500.$],
        lbl:<nool:s500>
      )][
        #subfigure(
          image(
            "./figures/noolstep1000.png"
          ),
          caption: [$italic("step") = 1000$.],
          lbl:<nool:s1000>
        )
      ]
      #grid(
      columns: (1fr, 1fr),
      gutter: -50pt
    )[
      #subfigure(
        image(
          "./figures/noolstep5000.png",
          width: 60%
        ),
        caption: [$italic("step") = 5000$.],
        lbl: <nool:s5000>
      )][
      #subfigure(
        image(
          "./figures/noolstep9900.png",
          width: 60%
        ),
        caption: [$italic("step") = 9900.$],
        lbl:<nool:s9900>
      )]
    ]<nool:parent>    
  ]

== Visualize the model
We choose the value of packing fraction equals 0.3 and $Delta x$ equals 0.15 to run the simulation and check the visual result using the Webgl code. The results are shown in @nool:parent.

== Estimate of packing fraction when the FCC crystal melts
To estimate the packing fraction, we will run the simulation using various packing fractions. 

FCC crystals have approximately 70% packing, this is the highest possible packing for spheres of the same size. For the melting situation, we choose to adjust the value of the packing fraction decreasing from 0.7 to 0.1, and we also adjust the $Delta x$ to make sure acceptance is around 0.5 in the meantime, as shown in @table1. 

#FigureBlock(
  captionAlign: "c",
)[
  #figure(
    placement: none,
    caption: [Outputs when _step_ $=100$.]
  )[
    #grid(
      columns: (1fr, 1fr, 1fr, 1fr),
      gutter: 10pt
    )[
      #subfigure(
        image(
          "./figures/pf0.7s100.png"
        ),
        caption: [$italic("P.F.") = 0.7$.],
        lbl: <pfcp:p7>
      )][
      #subfigure(
        image(
          "./figures/pf0.6s100.png"
        ),
        caption: [$italic("P.F.") = 0.6$.],
        lbl:<pfcp:p6>
      )][
        #subfigure(
          image(
            "./figures/pf0.5s100.png"
          ),
          caption: [$italic("P.F.") = 0.5$.],
          lbl:<pfcp:p5>
        )
      ][
        #subfigure(
          image(
            "./figures/pf0.4s100.png"
          ),
          caption: [$italic("P.F.") = 0.4$.],
          lbl:<pfcp:p4>
        )
      ]
      #grid(
      columns: (1fr, 1fr, 1fr),
      gutter: -50pt
    )[
      #subfigure(
        image(
          "./figures/pf0.3s100.png",
          width: 60%
        ),
        caption: [$italic("P.F.") = 0.3$.],
        lbl: <pfcp:p3>
      )][
      #subfigure(
        image(
          "./figures/pf0.2s100.png",
          width: 60%
        ),
        caption: [$italic("P.F.") = 0.2$.],
        lbl:<pfcp:p2>
      )][
      #subfigure(
        image(
          "./figures/pf0.1s100.png",
          width: 60%
        ),
        caption: [$italic("P.F.") = 0.1$.],
        lbl:<pfcp:p1>
      )]
    ]<pfcp:parent>    
  ]

From @pfcp:parent, we can see that the FCC crystal structure becomes disordered as the packing fraction continues to decrease. When the packing fraction is equal to 0.4, the crystal completely loses its ordered structure, which proves that the crystal melts from solid to liquid at this time. This point is the melting point.

// Next, we continue to simulate around the value x. We can see more accurately that when the crystal is at the melting point, the value of the packing fraction is: 


// -----------------------
#include "code.typ"