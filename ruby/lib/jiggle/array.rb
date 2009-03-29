class Array
  def each_pair(include_first_and_last = true)
    idx = 0
    while idx < self.length - 1
      yield self[idx], self[idx + 1]
      idx += 1
    end

    yield self[0], self[length - 1] if include_first_and_last and self.length > 2
  end
end
