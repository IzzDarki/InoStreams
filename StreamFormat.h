#pragma once
#ifndef INO_STREAMFORMAT_INCLUDED
#define INO_STREAMFORMAT_INCLUDED

#include "InoCore.h"
#include "Packed.h"

#include <inttypes.h>

namespace ino {

    //BaseFormats

    struct BaseFormats {
        uint8_t BaseVal;
        enum class Mode { NoFormat, PrefixFormat, FullFormat } BaseMode;

        constexpr inline BaseFormats(uint8_t BaseVal, Mode BaseMode = Mode::NoFormat) : BaseVal(BaseVal), BaseMode(BaseMode) {}

		constexpr inline bool operator==(const BaseFormats& Other) const { return BaseMode == Mode::NoFormat || (BaseVal == Other.BaseVal && BaseMode == Other.BaseMode); }
		constexpr inline bool operator!=(const BaseFormats& Other) const { return !(*this == Other); }
    };

    namespace Fmt {
        constexpr BaseFormats Bin = 2;
        constexpr BaseFormats Oct = 8;
        constexpr BaseFormats Dec = 10;
        constexpr BaseFormats Hex = 16;
    }

	template <typename T>
	using BaseFormat = Packed<T, BaseFormats>;

    template <typename T>
    constexpr inline auto Bin(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::Bin)) {
        return Pack(std::forward<T>(Var), Fmt::Bin);
    }

    template <typename T>
    constexpr inline auto Oct(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::Oct)) {
        return Pack(std::forward<T>(Var), Fmt::Oct);
    }

    template <typename T>
    constexpr inline auto Dec(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::Dec)) {
        return Pack(std::forward<T>(Var), Fmt::Dec);
    }

    template <typename T>
    constexpr inline auto Hex(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::Hex)) {
        return Pack(std::forward<T>(Var), Fmt::Hex);
    }

    template <typename T>
    constexpr inline auto Base(T &&Var, const BaseFormats &BaseNum) -> decltype(Pack(std::forward<T>(Var), BaseNum))  {
        return Pack(std::forward<T>(Var), BaseNum);
    }

    //CaseFormats

    enum class CaseFormats { Uppercase, Lowercase };

    namespace Fmt {
        constexpr CaseFormats Uppercase = CaseFormats::Uppercase;
        constexpr CaseFormats Lowercase = CaseFormats::Lowercase;
    }

	template <typename T>
	using CaseFormat = Packed<T, CaseFormats>;

    template <typename T>
    constexpr inline auto Uppercase(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::Uppercase)) {
        return Pack(std::forward<T>(Var), Fmt::Uppercase);
    }

    template <typename T>
    constexpr inline auto Lowercase(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::Lowercase)) {
        return Pack(std::forward<T>(Var), Fmt::Lowercase);
    }

    //BoolFormats

    struct BoolFormats {
        const char* BoolTrue;
        const char* BoolFalse;

        constexpr inline BoolFormats(const char* BoolTrue, const char* BoolFalse) : BoolTrue(BoolTrue), BoolFalse(BoolFalse) {}
    };

    namespace Fmt {
        constexpr BoolFormats BoolWord = { "true", "false" };
        constexpr BoolFormats BoolCapital = { "True", "False" };
        constexpr BoolFormats BoolCaps = { "TRUE", "FALSE" };
        constexpr BoolFormats BoolNum = { "1", "0" };
    }

	template <typename T>
	using BoolFormat = Packed<T, BoolFormats>;

	template <typename T>
    constexpr inline auto BoolWord(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::BoolWord)) {
        return Pack(std::forward<T>(Var), Fmt::BoolWord);
    }

    template <typename T>
    constexpr inline auto BoolCapital(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::BoolCapital)) {
        return Pack(std::forward<T>(Var), Fmt::BoolCapital);
    }

    template <typename T>
    constexpr inline auto BoolCaps(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::BoolCaps)) {
        return Pack(std::forward<T>(Var), Fmt::BoolCaps);
    }

    template <typename T>
    constexpr inline auto BoolNum(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::BoolNum)) {
        return Pack(std::forward<T>(Var), Fmt::BoolNum);
    }

    template <typename T>
    constexpr inline auto BoolUserDef(T&& Var, const BoolFormats& Bool) -> decltype(Pack(std::forward<T>(Var), Bool)) {
        return Pack(std::forward<T>(Var), Bool);
    }

    //DecimalpointFormats

    enum class DecimalpointFormats { Dot, Comma };

    namespace Fmt {
        constexpr DecimalpointFormats DecimalDot = DecimalpointFormats::Dot;
        constexpr DecimalpointFormats DecimalComma = DecimalpointFormats::Comma;
    }

	template <typename T>
	using DecimalpointFormat = Packed<T, DecimalpointFormats>;

	template <typename T>
    constexpr inline auto DecimalDot(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::DecimalDot)) {
        return Pack(std::forward<T>(Var), Fmt::DecimalDot);
    }

    template <typename T>
    constexpr inline auto DecimalComma(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::DecimalComma)) {
        return Pack(std::forward<T>(Var), Fmt::DecimalComma);
    }

    //PrecisionFormats

    struct PrecisionFormats {
        uint8_t PrecisionVal;

        constexpr inline PrecisionFormats(uint8_t PrecisionVal) : PrecisionVal(PrecisionVal) {}
    };

	template <typename T>
	using PrecisionFormat = Packed<T, PrecisionFormats>;

	template <typename T>
    constexpr inline auto Precision(T&& Var, const PrecisionFormats& Precision) -> decltype(Pack(std::forward<T>(Var), Precision)) {
        return Pack(std::forward<T>(Var), Precision);
    }

	//SpecialnumberFormat

	struct SpecialnumberFormats {
		const char* Nan;
		const char* PosInf;
		const char* NegInf;

		constexpr inline SpecialnumberFormats(const char* Nan, const char* PosInf, const char* NegInf) : Nan(Nan), PosInf(PosInf), NegInf(NegInf) {}
	};

	namespace Fmt {
		constexpr SpecialnumberFormats SpecialnumberShort = { "nan", "inf+", "inf-" };
		constexpr SpecialnumberFormats SpecialnumberLong = { "not-a-number", "positive-infinity", "negative-infinity" };
	}

	template <typename T>
	using SpecialnumberFormat = Packed<T, SpecialnumberFormats>;

	template <typename T>
	constexpr inline auto SpecialnumShort(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::SpecialnumberShort)) {
		return Pack(std::forward<T>(Var), Fmt::SpecialnumberShort);
	}

	template <typename T>
	constexpr inline auto SpecialnumLong(T&& Var) -> decltype(Pack(std::forward<T>(Var), Fmt::SpecialnumberLong)) {
		return Pack(std::forward<T>(Var), Fmt::SpecialnumberLong);
	}

	template <typename T>
	constexpr inline auto SpecialnumUserDef(T&& Var, const SpecialnumberFormats& Specialnum) -> decltype(Pack(std::forward<T>(Var), Specialnum)) {
		return Pack(std::forward<T>(Var), Specialnum);
	}

	//CStringFormats

	struct CStringFormats {
		enum class Mode { Exact, Max } StringMode;

		uint8_t StringSize;
		constexpr CStringFormats(uint8_t StringSize, Mode StringMode = Mode::Max) : StringMode(StringMode), StringSize(StringSize) {}
	};

	template <typename T>
	using CStringFormat = Packed<T, CStringFormats>;

	template <typename T>
	constexpr inline auto CString(T&& Var, const CStringFormats& CString) -> decltype(Pack(std::forward<T>(Var), CString)) {
		return Pack(std::forward<T>(Var), CString);
	}

	//Multiple formats

	template <typename VarT, typename... FmtTs>
	using MultiFormat = Packed<VarT, FmtTs...>;

	//template <typename... FmtTs>
	//struct IsMultiFormat : std::integral_constant<bool, sizeof...(FmtTs) >= 2> {};

	// note template <typename T>
	// note struct IsSTDFormat : std::integral_constant<bool,
	// note 	!std::is_same<T, BaseFormats>::value &&
	// note 	!std::is_same<T, CaseFormats>::value &&
	// note 	!std::is_same<T, BoolFormats>::value &&
	// note 	!std::is_same<T, DecimalpointFormats>::value &&
	// note 	!std::is_same<T, PrecisionFormats>::value>
	// note {};

	//template <typename... Ts>
	//struct IsOnlySTDFormat : std::integral_constant<bool, FoldLogicalAnd<IsSTDFormat<Ts>::value...>::value> {};

	template <typename VarT, typename... FmtTs>
	constexpr inline auto Format(VarT&& Var, const FmtTs&... FormatPars) -> decltype(Pack(std::forward<VarT>(Var), FormatPars...)) {
		return Pack(std::forward<VarT>(Var), FormatPars...);
	}

}

#endif
