local Stack = require("luacore").types.stack

describe("Stack", function()
	describe(":Push", function()
		it("should push items onto the stack", function()
			local s1 = Stack()

			s1:Push({ name = "Alice", role = "Developer" })
			s1:Push({ name = "Bob", title = "CTO" })
			assert.are.equals(2, s1:Size())

			local s2 = Stack()
			s2:Push({ name = "Charlie", company = "TechCorp" })
			s2:Push({ name = "Diana", skills = "Python" })
			s2:Push({ name = "Ethan", role = "Manager" })
			assert.are.equals(3, s2:Size())

			local s3 = Stack()
			assert.are.equals(0, s3:Size())

			local s4 = Stack()
			s4:Push({ name = "Frank", experience = "5 years" })
			s4:Push({ name = "Grace", education = "MBA" })
			s4:Push({ name = "Henry", location = "New York" })
			s4:Push({ name = "Ivy", industry = "Finance" })
			assert.are.equals(4, s4:Size())

			local s1 = Stack()
			s1:Push({ name = "Jack", connections = 500 })
			s1:Push({ name = "Kelly", endorsements = 50 })
			assert.are.equals(2, s1:Size())
		end)
	end)
end)
