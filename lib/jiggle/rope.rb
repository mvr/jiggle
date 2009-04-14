module Jiggle
  
  class Rope < Group
    Infinity = 1.0/0 # Duplicated
    
    def initialize(point_a, point_b, attr={})
      super()
      
      attr[:number_of_points] ||= 10
      attr[:mass] ||= attr[:number_of_points]
            
      mass = attr[:mass].to_f / attr[:number_of_points].to_f
      
      points = points_along point_a, point_b, attr[:number_of_points]

      @particles = points.map { |point| Particle.new point, :mass => mass }
      @particles.first.mass = Infinity
      @particles.last.mass = Infinity

      @springs = @particles.adjacent(false).map do |p1, p2|
        Spring.new p1, p2, :strength => attr[:strength], :damping => attr[:damping]
      end
    end

    protected

    def points_along(a, b, num)
      step = (b - a) / num
      points = []
      num.times do |i|
        points << a + (step * i)
      end
      points
    end
  end

  class World
    alias :add_rope    :add_group
    alias :remove_rope :remove_group
  end
  
end
