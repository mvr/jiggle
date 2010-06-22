require 'jiggle/body'

module Jiggle
  
  class FixedBody < Body
    Infinity = 1.0/0 # AGAIN.
    
    def initialize(points, attr={})
      super(points, attr)

      anchor_strength = attr[:anchor_strength] || 100
      anchor_damping  = attr[:anchor_damping]  || 40
      
      anchors = []
      springs = []
      
      @original_particles = @particles
      
      @particles.each do |particle|
        anchor = Particle.new particle.position, :mass => Infinity
        anchor.collidable = false
        anchors << anchor
        
        spring = Spring.new particle, anchor, :strength => anchor_strength,
                                              :damping => anchor_damping
        springs << spring
      end
      
      @particles += anchors
      @springs   += springs
      
    end

    def particles
      @original_particles
    end
  end
  
end
