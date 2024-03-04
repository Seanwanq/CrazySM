#import "@preview/cetz:0.2.1"
#import "../template.typ": *

#v(1em)
#FigureBlock(
  captionAlign: "c"
)[
  #figure(
    placement: none,
    caption: [Fraction Volume of a Cubic Lattice.]
  )[
  #scale(130%)[
    #v(1em)
    #cetz.canvas({
      import cetz.draw: *
      circle((0,0), radius: 2.0, fill: rgb("#bfdcf5"))
      rect((-2,-2), (2,2), fill: rgb(50, 50, 250, 50))
      line((0,0), (angle:45deg, radius: 2), name: "radius")
      content(
        "radius.mid",
        angle: "radius.end",
        padding: .1,
        anchor: "south",
        [$r$]
      )
      line((2.2,2),(2.4,2))
      line((2.2,-2),(2.4,-2))
      line((2.3,2),(2.3, 0.2))
      content((2.3,0),[$2r$])
      line((2.3, -0.2),(2.3, -2))
      
      line((-2, 2.2), (-2, 2.4))
      line((2, 2.2), (2, 2.4))
      line((-2, 2.3), (-0.2, 2.3))
      line((0.2, 2.3), (2, 2.3))
      content((0, 2.3), [$2r$])
      circle((0,0),radius: 0.05, fill: black)
    })
    #v(1.5em)
  ]
  ] <fractioncl>
]

