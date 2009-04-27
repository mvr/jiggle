module Jiggle

  class World
    attr_reader :particles
    attr_reader :springs
    attr_reader :areas
    
    def setup
      @last_time = Time.now
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
  end
  
end

  
