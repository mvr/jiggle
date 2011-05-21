require 'jiggle/group'

module Jiggle
  
  class Body < Group
    Infinity = 1.0/0 # WTF, Ruby?
    
    def initialize(points, attr={})
      super()

      unless attr[:static]
        attr[:mass] ||= points.size
        mass = attr[:mass] / points.size
      else
        mass = Infinity
      end
      
      scale  = attr[:scale]  || 1
      offset = attr[:offset] || jgv(0, 0)

      points = points.map { |point| (point * scale) + offset }
      
      points.each do |point|
        particle = Particle.new point, :mass => mass
        @particles << particle
      end

      outside = @particles.adjacent(true)
      inside  = @particles.pairs - outside
      
      outside.each do |p1, p2|
        spring = Spring.new p1, p2, :strength => attr[:strength], :damping => attr[:damping]
        @springs << spring
      end

      inside.each do |p1, p2|
        spring = Spring.new p1, p2, :strength => attr[:strength], :damping => attr[:damping]
        spring.strength /= 1.5
        @springs << spring
      end
      
      @areas = [Area.new(@particles, attr)]
    end
  end
end
