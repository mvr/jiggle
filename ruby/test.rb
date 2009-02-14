require 'jiggle'
include Jiggle
require 'rubygems'
require 'rubygame'
include Rubygame

class Array
  def each_pair
    each_with_index do |elt, idx|
      idx = if idx == length - 1
              0
            else
              idx + 1
            end
      yield elt, self[idx]
    end
  end
end

class Vector2
  def to_world
    (self - jgv(200, 200)) / 5
  end
  def to_screen
    (self * 5) + jgv(200, 200)
  end
  def to_s
    "(#{x}, #{y})"
  end
  def inspect
    "#<Jiggle::Vector2 x=#{x} y=#{y}>"
  end
end

Rubygame.init
screen = Screen.set_mode [400, 400]

ball = SpringBody.new([jgv(-1,  1),
                       jgv( 1,  1),
                       jgv( 1, -1),
                       jgv(-1, -1)],
                      :angle => 0,
                      :mass => 2,
                      :shape_matching => true,
                      :shape_k => 5 
                      )
ball.add_spring 0, 2, 50, 12
ball.add_spring 1, 3, 50, 12
ball.force jgv(50, 0)

# Paddles

paddle_shape = [jgv(-1, -10),
                jgv(-1,  10),
                jgv( 1,  10),
                jgv( 1, -10)]

left_paddle = SpringBody.new(paddle_shape,
                             :position => jgv(-15,0),
                             :kinematic => true,
                             :shape_matching => true)
left_paddle.add_spring 0, 2, 10, 15
left_paddle.add_spring 1, 3, 10, 15

right_paddle = SpringBody.new(paddle_shape,
                              :position => jgv(15,0),
                              :kinematic => true,
                              :shape_matching => true)
right_paddle.add_spring 0, 2, 10, 15
right_paddle.add_spring 1, 3, 10, 15

# Wall

wall_shape = [jgv(-20, -1),
              jgv(-20,  1),
              jgv( 20,  1),
              jgv( 20, -1)]

top_wall    = StaticBody.new(wall_shape,
                             :position => jgv(0, -20))
bottom_wall = StaticBody.new(wall_shape,
                             :position => jgv(0, 20))

w = World.new

w.add ball 
w.add left_paddle
w.add right_paddle
w.add top_wall
w.add bottom_wall

buffer = Surface.new(screen.size)

loop do
  buffer.fill :black

  w.bodies.each do |body|
    body.masses.map{|m| m.position}.each_pair do |v1, v2|
      v1 = v1.to_screen
      v2 = v2.to_screen
      buffer.draw_line_a [v1.x, v1.y], [v2.x, v2.y], :white
    end
  end

  ball.angle = 0
  ball.masses.each do |mass|
    mass.velocity *= 1.005
  end

  buffer.blit(screen, [0,0])
  w.update(1.0/120.0)
  screen.update
end

Rubygame.quit
