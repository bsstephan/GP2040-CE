import { create } from 'zustand';

import WebApi from '../Services/WebApi';

type State = {
	pins: { [key: string]: { buttonsMask: number } };
	loadingPins: boolean;
};

type Actions = {
	fetchPins: () => void;
	setPinMask: (pin: string, buttonsMask: number) => void;
	savePins: () => Promise<object>;
};

const INITIAL_STATE: State = {
	pins: {
		pin00: { buttonsMask: 0 },
		pin01: { buttonsMask: 0 },
		pin02: { buttonsMask: 0 },
		pin03: { buttonsMask: 0 },
		pin04: { buttonsMask: 0 },
		pin05: { buttonsMask: 0 },
		pin06: { buttonsMask: 0 },
		pin07: { buttonsMask: 0 },
		pin08: { buttonsMask: 0 },
		pin09: { buttonsMask: 0 },
		pin10: { buttonsMask: 0 },
		pin11: { buttonsMask: 0 },
		pin12: { buttonsMask: 0 },
		pin13: { buttonsMask: 0 },
		pin14: { buttonsMask: 0 },
		pin15: { buttonsMask: 0 },
		pin16: { buttonsMask: 0 },
		pin17: { buttonsMask: 0 },
		pin18: { buttonsMask: 0 },
		pin19: { buttonsMask: 0 },
		pin20: { buttonsMask: 0 },
		pin21: { buttonsMask: 0 },
		pin22: { buttonsMask: 0 },
		pin23: { buttonsMask: 0 },
		pin24: { buttonsMask: 0 },
		pin25: { buttonsMask: 0 },
		pin26: { buttonsMask: 0 },
		pin27: { buttonsMask: 0 },
		pin28: { buttonsMask: 0 },
		pin29: { buttonsMask: 0 },
	},
	loadingPins: false,
};

const useMultiPinStore = create<State & Actions>()((set, get) => ({
	...INITIAL_STATE,
	fetchPins: async () => {
		set({ loadingPins: true });
		const pins = await WebApi.getMultiPinMappings();
		set((state) => ({
			...state,
			pins,
			loadingPins: false,
		}));
	},
	setPinMask: (pin, buttonsMask) =>
		set((state) => ({
			...state,
			pins: { ...state.pins, [pin]: { buttonsMask } },
		})),
	savePins: async () => WebApi.setMultiPinMappings(get().pins),
}));

export default useMultiPinStore;
