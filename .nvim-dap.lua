local dap = require("dap")

dap.adapters.gdb = function(cb, config)
	cb({
		id = "gdb",
		type = "executable",
		command = vim.fs.normalize("~/apps/binutils-gdb/bin/gdb"),
		args = { "--quiet", "--interpreter=dap" },
	})
end

dap.adapters.codelldb = function(cb, config)
	cb({
		id = "codelldb",
		type = "executable",
		command = "codelldb",
	})
end

local prg = vim.fs.normalize("build/Debug/crawl")

dap.configurations.c = {
	{
		-- The first three options are required by nvim-dap
		type = "codelldb", -- the type here established the link to the adapter definition: `dap.adapters.python`
		request = "launch",
		name = "crawl-basic",

		program = prg, -- This configuration will launch the current file if used
		-- args = { "-o", "out/", "https://c-faq.com/decl/index.html" },
		args = { "https://c-faq.com/decl/index.html" },
	},
}
