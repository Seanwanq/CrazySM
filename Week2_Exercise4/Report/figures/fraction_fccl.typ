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
    #cetz.canvas({
      import cetz.draw: *
      let circleColor = rgb("#bfdcf5")

      circle((0,0), radius: 2.0, fill: circleColor, name: "ccircle")
      
      arc((2 * calc.sqrt(2) - 2, 2 * calc.sqrt(2)),radius: 2, start: 180deg, stop: 270deg, mode: "PIE", fill: circleColor)

      arc((-2 * calc.sqrt(2) + 2, 2 * calc.sqrt(2)),radius: 2, start: 0deg, stop: -90deg, mode: "PIE", fill: circleColor, name: "lcircle")

      arc((2 * calc.sqrt(2) - 2, -2 * calc.sqrt(2)),radius: 2, start: 180deg, stop: 90deg, mode: "PIE", fill: circleColor)

      arc((-2 * calc.sqrt(2) + 2, -2 * calc.sqrt(2)),radius: 2, start: 0deg, stop: 90deg, mode: "PIE", fill: circleColor)
      
      rect((-2 * calc.sqrt(2),-2 * calc.sqrt(2)), (2 * calc.sqrt(2), 2 * calc.sqrt(2)), fill: rgb(50, 50, 250, 50))

      line((0,0), (angle:30deg, radius: 2), name: "radius")
      
      content(
        "radius.mid",
        angle: "radius.end",
        padding: .1,
        anchor: "south",
        [$r$]
      )
      
      let lengthHalf = 2 * calc.sqrt(2)

      line((lengthHalf + 0.4, lengthHalf),(lengthHalf + 0.6, lengthHalf))
      line((lengthHalf + 0.4, -lengthHalf),(lengthHalf + 0.6, -lengthHalf))
      line((lengthHalf + 0.5, lengthHalf),(lengthHalf + 0.5, 0.3))
      content((lengthHalf + 0.6,0),[$2 sqrt(2) r$])
      line((lengthHalf + 0.5, -0.3),(lengthHalf + 0.5, -lengthHalf))
      
      line((-lengthHalf, lengthHalf + 0.4), (-lengthHalf, lengthHalf + 0.6))
      line((lengthHalf, lengthHalf + 0.4), (lengthHalf, lengthHalf + 0.6))
      line((-lengthHalf, lengthHalf + 0.5), (-0.6, lengthHalf + 0.5))
      line((0.6, lengthHalf + 0.5), (lengthHalf, lengthHalf + 0.5))
      content((0, lengthHalf + 0.5), [$2 sqrt(2) r$])
      circle((0,0),radius: 0.05, fill: black)
      
      line((-lengthHalf - 0.4, lengthHalf), (-lengthHalf - 0.2, lengthHalf))
      line((-lengthHalf - 0.4, lengthHalf - 2), (-lengthHalf - 0.2, lengthHalf - 2))
      line((-lengthHalf - 0.3, lengthHalf), (-lengthHalf - 0.3, lengthHalf - 2))
      content((-lengthHalf - 0.5, lengthHalf - 1), [$r$])
      line((-lengthHalf,lengthHalf), (name: "lcircle", anchor: 30deg), name: "radius2")
      content(
        "radius2.mid",
        padding: .1,
        anchor: "south",
        [$r$]
      )
    })
  ] <fractionfccl>
]

