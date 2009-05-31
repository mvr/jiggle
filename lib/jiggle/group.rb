module Jiggle
  
  class Group
    attr_accessor :particles
    attr_accessor :springs
    attr_accessor :areas

    def initialize
      @particles = []
      @springs = []
      @areas = []
    end

    def objects
      [@particles, @springs, @areas].flatten
    end

    def contains?(thing)
      @areas.any? {|a| a.contains? point}
    end

    def elasticity=(value)
      [@particles, @areas].flatten.each do |p|
        p.elasticity = value
      end
    end

    def friction=(value)
      [@particles, @areas].flatten.each do |p|
        p.friction = value
      end
    end
    
    def expand(amount = 2)
      @springs.each do |spring|
        spring.length *= amount
      end
    end
    def contract(amount = 2)
      @springs.each do |spring|
        spring.length /= amount
      end
    end
    
  end

  class World
    def add_group(group)
      group.objects.each do |o|
        add o
      end
    end
    
    def remove_group(group)
      group.objects.each do |o|
        remove o
      end
    end
  end
  
end
