require "thats_it/version"
require "thats_it/thats_it"

module ThatsIt
  CALL_TP   = TracePoint.trace(:call)   { |tp| setup_it_block_call }
  C_CALL_TP = TracePoint.trace(:c_call) { |tp| setup_it_block_c_call }
end
