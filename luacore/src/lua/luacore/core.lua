local M = {}

function M.Panic(msg)
	error(msg or "", 2)
end

return M
