import React, {
	useCallback,
	useContext,
	useEffect,
	useMemo,
	useState,
} from 'react';
import Select from 'react-select';
import { Alert, Button, Form } from 'react-bootstrap';
import { useTranslation } from 'react-i18next';
import zip from 'lodash/zip';

import { AppContext } from '../Contexts/AppContext';
import Section from '../Components/Section';

import { BUTTON_MASKS, getButtonLabels } from '../Data/Buttons';
import useMultiPinStore, { MaskPayload } from '../Store/useMultiPinStore';

const FILTERED_BUTTON_MASKS = BUTTON_MASKS.filter(
	({ label }) => label !== 'None',
);

type PinCell = [string, MaskPayload];
type PinRow = [PinCell, PinCell];
type PinList = [PinRow];

export default function MultiMappingPage() {
	const { fetchPins, pins, savePins, setPinMask } = useMultiPinStore();
	const { updateUsedPins } = useContext(AppContext);
	const [saveMessage, setSaveMessage] = useState('');

	const { t } = useTranslation('');

	useEffect(() => {
		fetchPins();
	}, []);

	const handleSubmit = async (e) => {
		e.preventDefault();
		e.stopPropagation();
		try {
			await savePins();
			updateUsedPins();
			setSaveMessage(t('Common:saved-success-message'));
		} catch (error) {
			setSaveMessage(t('Common:saved-error-message'));
		}
	};

	const pinList = useMemo<PinList>(() => {
		const pinArray = Object.entries(pins);
		return zip(
			pinArray.slice(0, pinArray.length / 2),
			pinArray.slice(pinArray.length / 2, pinArray.length),
		);
	}, [pins]);

	const createCell = useCallback(
		([pin, pinValue]: PinCell) => (
			<div className="d-flex col py-2">
				<div
					className="d-flex align-items-center flex-shrink-0"
					style={{ width: '4rem' }}
				>
					<label htmlFor={pin}>{pin.toUpperCase()}</label>
				</div>
				<Select
					inputId={pin}
					className="text-primary flex-grow-1"
					isClearable
					isSearchable
					options={FILTERED_BUTTON_MASKS}
					placeholder={t('MultiMapping:placeholder')} // TODO translation
					value={FILTERED_BUTTON_MASKS.filter(
						({ value }) => pinValue.customButtonMask & value,
					)}
					isMulti
					onChange={(selected) =>
						setPinMask(
							pin,
							selected.reduce(
								(mask, option) => (option?.value ? mask ^ option.value : mask),
								0,
							),
						)
					}
				/>
			</div>
		),
		[],
	);

	return (
		<>
			<Section title={t('MultiMapping:header-text')}>
				<Form onSubmit={handleSubmit}>
					<div className="gx-3">
						{pinList.map(([cell1, cell2], i) => (
							<div key={`pin-row-${i}`} className="row">
								{createCell(cell1)}
								{createCell(cell2)}
							</div>
						))}
					</div>
					<Button type="submit" className="my-4">
						{t('Common:button-save-label')}
					</Button>
					{saveMessage && <Alert variant="secondary">{saveMessage}</Alert>}
				</Form>
			</Section>
		</>
	);
}
