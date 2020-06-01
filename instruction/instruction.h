#pragma once

#include <cstdint>
#include <utility>
#include <unordered_map>
#include <numeric>
#include <vector>

namespace c0 {
	enum Operation {
		NOP = 0x00,
		BIPUSH = 0x01,
		IPUSH = 0x02,
		POP = 0x04,
		POP2 = 0x05,
		POPN = 0x06,
		DUP = 0x07,
		DUP2 = 0x08,
		LOADC = 0x09,
		LOADA = 0x0a,
		NEW = 0x0b,
		SNEW = 0x0c,
		ILOAD = 0x10,
		DLOAD = 0x11,
		ALOAD = 0x12,
		IALOAD = 0x18,
		DALOAD = 0x19,
		AALOAD = 0x1a,
		ISTORE = 0x20,
		DSTORE = 0x21,
		ASTORE = 0x22,
		IASTORE = 0x28,
		DASTORE = 0x29,
		AASTORE = 0x2a,
		IADD = 0x30,
		DADD = 0x31,
		ISUB = 0x34,
		DSUB = 0x35,
		IMUL = 0x38,
		DMUL = 0x39,
		IDIV = 0x3c,
		DDIV = 0x3d,
		INEG = 0x40,
		DNEG = 0x41,
		ICMP = 0x44,
		DCMP = 0x45,
		I2D = 0x60,
		D2I = 0x61,
		I2C = 0x62,
		JMP = 0x70,
		JE = 0x71,
		JNE = 0x72,
		JL = 0x73,
		JGE = 0x74,
		JG = 0x75,
		JLE = 0x76,
		CALL = 0x80,
		RET = 0x88,
		IRET = 0x89,
		DRET = 0x8a,
		ARET = 0x8b,
		IPRINT = 0xa0,
		DPRINT = 0xa1,
		CPRINT = 0xa2,
		SPRINT = 0xa3,
		PRINTL = 0xaf,
		ISCAN = 0xb0,
		DSCAN = 0xb1,
		CSCAN = 0xb2,
	};
	const std::unordered_map<Operation, std::vector<int>> paramSizeOfOpCode = {
	{ Operation::BIPUSH, {1} },    { Operation::IPUSH, {4} },
	{ Operation::POPN, {4} },
	{ Operation::LOADC, {2} },    { Operation::LOADA, {2, 4} },
	{ Operation::SNEW, {4} },

	{ Operation::JMP, {2} },
	{ Operation::JE, {2} }, { Operation::JNE, {2} }, { Operation::JL, {2} }, { Operation::JGE, {2} }, { Operation::JG, {2} }, { Operation::JLE, {2} },

	{ Operation::CALL, {2} },
	};
	class Instruction final {
	private:
		using int32_t = std::int32_t;
	public:
		friend void swap(Instruction& lhs, Instruction& rhs);
	public:
		Instruction(Operation opr, int32_t p1, int32_t p2, uint16_t offset) : _opr(opr), _p1(p1), _p2(p2),_offset(offset) {}
		Instruction(Operation opr, int32_t p1, uint16_t offset) : _opr(opr), _p1(p1), _offset(offset) {}
		Instruction(Operation opr, uint16_t offset) : _opr(opr), _offset(offset) {}
		Instruction(uint16_t offset) : _opr(Operation::NOP), _offset(offset) {}
		Instruction(const Instruction& i) {
			_opr = i._opr; _p1 = i._p1; _p2 = i._p2;
			_offset = i._offset;
		}
		Instruction& operator=(Instruction i) { swap(*this, i); return *this; }
		bool operator==(const Instruction& i) const { return _opr == i._opr && _p1 == i._p1 && _p2 == i._p2&&_offset==i._offset; }

		Operation GetOperation() const { return _opr; }
		int32_t GetP1() const { return _p1; }
		int32_t GetP2() const { return _p2; }
		uint16_t GetOffset() const { return _offset; }
		uint16_t Getsize() const {
			return 1;
			//OhShit!quan dou bai xie le.....
			/*if (auto it = c0::paramSizeOfOpCode.find(_opr); it != c0::paramSizeOfOpCode.end()) {
				auto paramSizes = it->second;
				return std::accumulate(paramSizes.begin(), paramSizes.end(), 1);
			}*/
			//switch (_opr)
			//{
			//case c0::NOP:
			//	return 1;
			//	break;
			//case c0::BIPUSH:
			//	return 2;
			//	break;
			//case c0::IPUSH:
			//	return 5;
			//	break;
			//case c0::POP:
			//	return 1;
			//	break;
			//case c0::POP2:
			//	return 1;
			//	break;
			//case c0::POPN:
			//	return 5;
			//	break;
			//case c0::DUP:
			//	return 1;
			//	break;
			//case c0::DUP2:
			//	return 1;
			//	break;
			//case c0::LOADC:
			//	return 3;
			//	break;
			//case c0::LOADA:
			//	return 7;
			//	break;
			//case c0::NEW:
			//	return 1;
			//	break;
			//case c0::SNEW:
			//	return 5;
			//	break;
			//case c0::ILOAD:
			//	return 1;
			//	break;
			//case c0::DLOAD:
			//	return 1;
			//	break;
			//case c0::ALOAD:
			//	return 1;
			//	break;
			//case c0::IALOAD:
			//	return 1;
			//	break;
			//case c0::DALOAD:
			//	return 1;
			//	break;
			//case c0::AALOAD:
			//	return 1;
			//	break;
			//case c0::ISTORE:
			//	return 1;
			//	break;
			//case c0::DSTORE:
			//	return 1;
			//	break;
			//case c0::ASTORE:
			//	return 1;
			//	break;
			//case c0::IASTORE:
			//	return 1;
			//	break;
			//case c0::DASTORE:
			//	return 1;
			//	break;
			//case c0::AASTORE:
			//	return 1;
			//	break;
			//case c0::IADD:
			//	return 1;
			//	break;
			//case c0::DADD:
			//	return 1;
			//	break;
			//case c0::ISUB:
			//	return 1;
			//	break;
			//case c0::DSUB:
			//	return 1;
			//	break;
			//case c0::IMUL:
			//	return 1;
			//	break;
			//case c0::DMUL:
			//	return 1;
			//	break;
			//case c0::IDIV:
			//	return 1;
			//	break;
			//case c0::DDIV:
			//	return 1;
			//	break;
			//case c0::INEG:
			//	return 1;
			//	break;
			//case c0::DNEG:
			//	return 1;
			//	break;
			//case c0::ICMP:
			//	return 1;
			//	break;
			//case c0::DCMP:
			//	return 1;
			//	break;
			//case c0::I2D:
			//	return 1;
			//	break;
			//case c0::D2I:
			//	return 1;
			//	break;
			//case c0::I2C:
			//	return 1;
			//	break;
			//case c0::JMP:
			//	return 3;
			//	break;
			//case c0::JE:
			//	return 3;
			//	break;
			//case c0::JNE:
			//	return 3;
			//	break;
			//case c0::JL:
			//	return 3;
			//	break;
			//case c0::JGE:
			//	return 3;
			//	break;
			//case c0::JG:
			//	return 3;
			//	break;
			//case c0::JLE:
			//	return 3;
			//	break;
			//case c0::CALL:
			//	return 3;
			//	break;
			//case c0::RET:
			//	return 1;
			//	break;
			//case c0::IRET:
			//	return 1;
			//	break;
			//case c0::DRET:
			//	return 1;
			//	break;
			//case c0::ARET:
			//	return 1;
			//	break;
			//case c0::IPRINT:
			//	return 1;
			//	break;
			//case c0::DPRINT:
			//	return 1;
			//	break;
			//case c0::CPRINT:
			//	return 1;
			//	break;
			//case c0::SPRINT:
			//	return 1;
			//	break;
			//case c0::PRINTL:
			//	return 1;
			//	break;
			//case c0::ISCAN:
			//	return 1;
			//	break;
			//case c0::DSCAN:
			//	return 1;
			//	break;
			//case c0::CSCAN:
			//	return 1;
			//	break;
			//default:
			//	//impossible
			//	break;
			//}
		}
		void SetP(int32_t p1) {
			_p1 = p1;
			return;
		}
		void SetP(int32_t p1, int32_t p2) {
			_p1 = p1;
			_p2 = p2;
			return;
		}
	public:
		Operation _opr;
		uint16_t _offset;
		int32_t _p1;
		int32_t _p2;
	};

	inline void swap(Instruction& lhs, Instruction& rhs) {
		using std::swap;
		swap(lhs._opr, rhs._opr);
		swap(lhs._p1, rhs._p1);
		swap(lhs._p2, rhs._p2);
		swap(lhs._offset, rhs._offset);
	}
}