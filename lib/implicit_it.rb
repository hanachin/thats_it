require "implicit_it/version"
require "implicit_it/implicit_it"

module ImplicitIt
  # Your code goes here...
end

TracePoint.trace(:call) do |tp|
  setup_it_block
end
