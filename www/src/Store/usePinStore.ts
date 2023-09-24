import axios from 'axios';
import { create } from 'zustand';
import { baseUrl } from '../Services/WebApi';

// Hide from select options / Disable select if returned from board
export const NON_SELECTABLE_BUTTON_ACTIONS = [-5, 0];

// These could theoretically be created from enums.proto
export const BUTTON_ACTIONS = {
	NONE: -10,
	RESERVED: -5,
	ASSIGNED_TO_ADDON: 0,
	BUTTON_PRESS_UP: 1,
	BUTTON_PRESS_DOWN: 2,
	BUTTON_PRESS_LEFT: 3,
	BUTTON_PRESS_RIGHT: 4,
	BUTTON_PRESS_B1: 5,
	BUTTON_PRESS_B2: 6,
	BUTTON_PRESS_B3: 7,
	BUTTON_PRESS_B4: 8,
	BUTTON_PRESS_L1: 9,
	BUTTON_PRESS_R1: 10,
	BUTTON_PRESS_L2: 11,
	BUTTON_PRESS_R2: 12,
	BUTTON_PRESS_S1: 13,
	BUTTON_PRESS_S2: 14,
	BUTTON_PRESS_A1: 15,
	BUTTON_PRESS_A2: 16,
	BUTTON_PRESS_L3: 17,
	BUTTON_PRESS_R3: 18,
	BUTTON_PRESS_FN: 19,
	BUTTON_PRESS_DDI_UP: 20,
	BUTTON_PRESS_DDI_DOWN: 21,
	BUTTON_PRESS_DDI_LEFT: 22,
	BUTTON_PRESS_DDI_RIGHT: 23,
} as const;

type PinActionKeys = keyof typeof BUTTON_ACTIONS;
type PinActionValues = (typeof BUTTON_ACTIONS)[PinActionKeys];

type PinState = {
	pins: { [key: number]: PinActionValues };
	fetchPins: () => void;
	validate: () => void;
	setPinAction: (pin: number, action: PinActionValues) => void;
	savePins: () => void;
};

const usePinStore = create<PinState>()((set, get) => ({
	pins: {
		pin00: -10,
		pin01: -10,
		pin02: -10,
		pin03: -10,
		pin04: -10,
		pin05: -10,
		pin06: -10,
		pin07: -10,
		pin08: -10,
		pin09: -10,
		pin10: -10,
		pin11: -10,
		pin12: -10,
		pin13: -10,
		pin14: -10,
		pin15: -10,
		pin16: -10,
		pin17: -10,
		pin18: -10,
		pin19: -10,
		pin20: -10,
		pin21: -10,
		pin22: -10,
		pin23: -10,
		pin24: -10,
		pin25: -10,
		pin26: -10,
		pin27: -10,
		pin28: -10,
		pin29: -10,
	},
	fetchPins: async () => {
		const { data } = await axios.get(`${baseUrl}/api/getPinMappings`);
		set((state) => ({
			...state,
			pins: { ...state.pins, ...data }, // Merge saved pins with defaults
		}));
	},
	validate: () => {}, // Run yup on pins
	setPinAction: (pin, action) =>
		set((state) => ({
			...state,
			pins: { ...state.pins, [pin]: action },
		})),
	savePins: async () => {
		await axios
			.post(`${baseUrl}/api/setPinMappings`, get().pins)
			.then((response) => {
				console.log(response.data);
			})
			.catch((err) => {
				console.error(err);
			});
	},
}));

export default usePinStore;