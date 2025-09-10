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

dap.configurations.c = {
	{
		-- The first three options are required by nvim-dap
		type = "codelldb", -- the type here established the link to the adapter definition: `dap.adapters.python`
		request = "launch",
		name = "crawl-basic",

		program = vim.fs.normalize("build/Debug/crawl"), -- This configuration will launch the current file if used
		-- args = { "-o", "out/", "https://c-faq.com/decl/index.html" },
		-- args = { "https://c-faq.com/decl/index.html" },
		args = { "https://www.cl.cam.ac.uk/~mgk25/ucs/" },
	},
	{
		-- The first three options are required by nvim-dap
		type = "codelldb", -- the type here established the link to the adapter definition: `dap.adapters.python`
		request = "launch",
		name = "test-vec",

		program = vim.fs.normalize("build/Debug/test-vec"), -- This configuration will launch the current file if used
	},
	{
		-- The first three options are required by nvim-dap
		type = "codelldb", -- the type here established the link to the adapter definition: `dap.adapters.python`
		request = "launch",
		name = "test-veclist",

		program = vim.fs.normalize("build/Debug/test-veclist"), -- This configuration will launch the current file if used
	},
}
