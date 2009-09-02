module Jiggle

  class Space
    attr_reader :particles
    attr_reader :springs
    attr_reader :areas
    
    def setup
      @start_time = @last_time = Time.now
      @time_accumulator = 0
      @time_step = 1 / @ticks_per_second
    end
    private :setup
    
    def update
      clear_collisions
      
      now = Time.now
      difference = now - @last_time
      @last_time = now
      @time_accumulator += difference
      
      while @time_accumulator > @time_step
        step(@time_step)
        @time_accumulator -= @time_step
      end
    end

    def time
       @last_time - @start_time
    end
    
    def add(thing)
      case thing
      when Particle
        add_particle thing
      when Spring
        add_spring thing
      when Area
        add_area thing
      when Group
        add_group thing
      end
    end
    alias :<< :add
    
    def remove(thing)
      case thing
      when Particle
        remove_particle thing
      when Spring
        remove_spring thing
      when Area
        remove_area thing
      when Group
        remove_group thing
      end
    end
  end
  
end

  
